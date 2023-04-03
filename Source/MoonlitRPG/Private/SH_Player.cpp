// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Player.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "MoveComponent.h"
#include <Camera/CameraComponent.h>
#include "InventoryComponent.h"
#include "AttackComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerMainWG.h"
#include "SH_PlayerAnim.h"
#include "InteractiveObjectBase.h"
#include <Animation/AnimMontage.h>
#include <Components/CapsuleComponent.h>
#include "IH_DieUI.h"
#include "IH_LoadingUI.h"
#include "IH_WarpPoint.h"
#include "NPCBase.h"
#include "MainDialogueUI.h"
#include "IH_WarningUI.h"
#include <Engine/SkeletalMesh.h>
#include "QuestComponent.h"
#include "IH_PuzzleGuide.h"
#include <Particles/ParticleSystemComponent.h>
#include "ScreenShotUI.h"
#include <Kismet/GameplayStatics.h>
#include "PuzzleGuide.h"
#include "IH_DamageActor.h"
#include <UMG/Public/Blueprint/UserWidget.h>


ASH_Player::ASH_Player()
{
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	CamComp->SetupAttachment(SpringArmComp);

	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	EquippedComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp"));
	EquippedComp1->SetupAttachment(GetMesh(), TEXT("Weapon_Socket_1"));
	EquippedComp1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EquippedComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp2"));
	EquippedComp2->SetupAttachment(GetMesh(), TEXT("Weapon_Socket_2"));
	EquippedComp2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InvenComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InvenComp"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComp"));
	QuestComp = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComp"));

	
	ConstructorHelpers::FObjectFinder<UParticleSystem>tempHit(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Impact_2.P_Impact_2'"));
	if (tempHit.Succeeded())
	{
		hitImpact = tempHit.Object;
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrint/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimClass(tempAnim.Class);
		
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_F.Key_F'"));
	if (tempAction.Succeeded())
	{
		fkey = tempAction.Object; //f
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAlt(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_Alt.Key_Alt'"));
	if (tempAlt.Succeeded())
	{
		altkey = tempAlt.Object; //alt
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempWeapon1(TEXT("/Script/Engine.StaticMesh'/Game/Animation/Meshes/sword.sword'"));
	if (tempWeapon1.Succeeded())
	{
		weaponMesh.Add(tempWeapon1.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempWeapon2(TEXT("/Script/Engine.StaticMesh'/Game/Animation/Meshes/Dagger.Dagger'"));
	if (tempWeapon2.Succeeded())
	{
		weaponMesh.Add(tempWeapon2.Object);
	}

	SkeletalMeshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter_WGS.SK_SdCharacter_WGS'"));
	SkeletalMeshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter_WTD.SK_SdCharacter_WTD'"));
	SkeletalMeshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter_LS_fishingrob.SK_SdCharacter_LS_fishingrob'"));
	SkeletalMeshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter.SK_SdCharacter'"));

	dieUI = CreateWGClass<UIH_DieUI>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Die.WG_Die_C'"));
	loadingUI = CreateWGClass<UIH_LoadingUI>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Loading.WG_Loading_C'"));
	dialogueUI = CreateWGClass<UMainDialogueUI>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Dialogue.WG_Dialogue_C'"));
	warningUI = CreateWGClass<UIH_WarningUI>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_FightWarning.WG_FightWarning_C'"));
	MainHUD = CreateWGClass<UPlayerMainWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_WG_PlayerMain.BP_WG_PlayerMain_C'"));
	screenshotUI = CreateWGClass<UScreenShotUI>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_ScreenShot.WG_ScreenShot_C'"));

}

template<typename T>
T* ASH_Player::CreateWGClass(FString path)
{
	TSubclassOf<T> WGFactory;
	ConstructorHelpers::FClassFinder<T> tempWG(*path);
	if (tempWG.Succeeded())
	{
		WGFactory = tempWG.Class;
	}
	return CreateWidget<T>(GetWorld(), WGFactory);
}

void ASH_Player::SkeletalMeshFinder(FString path)
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(*path);
	if (tempMesh.Succeeded())
	{
		PlayerMesh.Add(tempMesh.Object);
	}
}

void ASH_Player::BeginPlay()
{
	Super::BeginPlay();

	MainHUD->AddToViewport();
	playerAnim = Cast<USH_PlayerAnim>(GetMesh()->GetAnimInstance());

	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));

	playerCon = GetWorld()->GetFirstPlayerController();
	playerCon->PlayerCameraManager->ViewPitchMin = -30.0f;
	playerCon->PlayerCameraManager->ViewPitchMax = 60.0f;


}

void ASH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttackComp->hitAnything)
	{
		float randomShake = FMath::RandRange(-2.0f, 2.0f);

		currTime += DeltaTime;
		if (currTime < 0.1f)
		{
			CamComp->SetWorldLocation(CamComp->GetComponentLocation()+FVector(randomShake));
		}
		else
		{
			currTime = 0;
			AttackComp->hitAnything=false;
			CamComp->SetRelativeLocation(FVector(-200, 0, 50));
		}
	}

}

void ASH_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent != nullptr)
	{
		EnhancedInputComponent->BindAction(fkey, ETriggerEvent::Triggered, this, &ASH_Player::interactionObject); //F키
		EnhancedInputComponent->BindAction(altkey, ETriggerEvent::Triggered, this, &ASH_Player::OpenScreenshotUI); //ALT키
		MoveComp->SetupPlayerInputComponent(EnhancedInputComponent);
		InvenComp->SetupPlayerInputComponent(EnhancedInputComponent);
		AttackComp->SetupPlayerInputComponent(EnhancedInputComponent);
		QuestComp->SetupPlayerInputComponent(EnhancedInputComponent);
	}
}

void ASH_Player::OpenScreenshotUI()
{
	if (!screenShotOpen)
	{
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetCharacterMovement()->DisableMovement();
		screenshotUI->AddToViewport();
		screenShotOpen = true;
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		screenshotUI->RemoveFromParent();
		screenShotOpen = false;
	}
}

void ASH_Player::interactionObject()
{
	FHitResult hitinfo;
	FCollisionShape interColli = FCollisionShape::MakeSphere(120.0f);
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);
	bool bhit = GetWorld()->SweepSingleByObjectType(hitinfo, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel3, interColli, param);

	FVector collisionLoc = GetActorLocation();
	collisionLoc.Z = 0;

	if (bhit)
	{
		
		AItemBase* curritem = Cast<AItemBase>(hitinfo.GetActor());
		if (curritem != nullptr)
		{
			curritem->GetItem();
			return;
		}
		
		AInteractiveObjectBase* currobject = Cast<AInteractiveObjectBase>(hitinfo.GetActor());
		if (currobject != nullptr)
		{
			currobject->Interaction();
			return;
		}

		ANPCBase* currNPC = Cast<ANPCBase>(hitinfo.GetActor());
		if (currNPC != nullptr)
		{
			if (QuestComp->MainQuest->SubType == ESubQuestType::Contact)
			{
				QuestComp->CheackRequirementTarget(currNPC->idx);
			}
			currNPC->InteractNPC();
			return;
		
		}

		APuzzleGuide* currGuide = Cast<APuzzleGuide>(hitinfo.GetActor());
		if (currGuide != nullptr)
		{
			currGuide->MoveToPuzzleActor();
			return;
		}
	}
}


void ASH_Player::DamagedPlayer(float DamageValue)
{
	if (!AttackComp->isSpecialAttacking)
	{
		PlayercurrHP -= DamageValue;
		if (PlayercurrHP >= DamageValue)
		{
			int32 randNum = FMath::RandRange(0, 1);
			FString sectionName = FString::Printf(TEXT("Damaged%d"), randNum);
			PlayAnimMontage(AttackComp->damagedMontage, 1.0f, FName(*sectionName));

			FloatingPlayerDamage();
			damageUI->UpdateDamage(DamageValue);
			damageUI->FloatingAnimation();

			GetCharacterMovement()->DisableMovement();
		}
		else // 플레이어 죽음
		{
			PlayAnimMontage(AttackComp->damagedMontage, 1.0f, FName(TEXT("Die")));
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			DisableInput(playerCon);
		}
		PlayercurrHP = FMath::Clamp(PlayercurrHP, 0, PlayerTotalHP);
		MainHUD->UpdateHP(PlayercurrHP, PlayerTotalHP);
	}
}

void ASH_Player::HealPlayer(float HealValue)
{
	PlayercurrHP += HealValue;
	PlayercurrHP = FMath::Clamp(PlayercurrHP, 0, PlayerTotalHP);
	MainHUD->UpdateHP(PlayercurrHP, PlayerTotalHP);
}

void ASH_Player::RevivePlayer()
{
	if (warpPoint == nullptr)
	{
		SetActorLocation(FVector(0, 0, 100));
		SetActorRotation(FRotator(0, 0, 0));
	}
	else
	{
		SetActorLocation(warpPoint->compSpawnPos->GetComponentLocation()+warpPoint->compSpawnPos->GetUpVector()*45);
		SetActorRotation(warpPoint->compSpawnPos->GetComponentRotation());
		dieUI->minDist = 10000;
	}

	HealPlayer(PlayerTotalHP / 3);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	EnableInput(playerCon);

	UAnimInstance* currAnim = GetMesh()->GetAnimInstance();
	if (currAnim != nullptr)
	{
		UAnimMontage* currMont = currAnim->GetCurrentActiveMontage();
		if (currMont != nullptr)
		{
			currAnim->Montage_Stop(0, currMont);
		}
	}
}

void ASH_Player::FloatingPlayerDamage()
{
	float randLocX = FMath::RandRange(-15, 15);
	damageUI = GetWorld()->SpawnActor<AIH_DamageActor>(damageActor, GetActorLocation() + GetActorRightVector() * randLocX, GetActorRotation());
}

void ASH_Player::SkillCameraFollow(USceneComponent* attachLoc, FName socketName)
{
	SpringArmComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SpringArmComp->AttachToComponent(attachLoc, FAttachmentTransformRules::KeepWorldTransform, socketName);
}

void ASH_Player::CompleteQuest(FQuestInfo Questinfo)
{
	MainHUD->UpdateQuestSummary(Questinfo);
	dialogueUI->SetStartDialouge(Questinfo.DialougueIndex);
}
