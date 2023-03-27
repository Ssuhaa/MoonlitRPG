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

	InvenComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InvenComp"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComp"));
	QuestComp = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComp"));

	ConstructorHelpers::FClassFinder<UPlayerMainWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_WG_PlayerMain.BP_WG_PlayerMain_C'"));
	if (tempWG.Succeeded())
	{
		MainWGFactory = tempWG.Class; 
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
	ConstructorHelpers::FClassFinder<UIH_DieUI> tempdieUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Die.WG_Die_C'"));
	if (tempdieUI.Succeeded())
	{
		UIFactory.Add(tempdieUI.Class);
	}
	ConstructorHelpers::FClassFinder<UIH_LoadingUI> temploadingUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Loading.WG_Loading_C'"));
	if (temploadingUI.Succeeded())
	{
		UIFactory.Add(temploadingUI.Class);
	}
	ConstructorHelpers::FClassFinder<UMainDialogueUI> tempdialogueUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Dialogue.WG_Dialogue_C'"));
	if (tempdialogueUI.Succeeded())
	{
		UIFactory.Add(tempdialogueUI.Class);
	}
	ConstructorHelpers::FClassFinder<UIH_WarningUI> tempwarningUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_FightWarning.WG_FightWarning_C'"));
	if (tempwarningUI.Succeeded())
	{
		UIFactory.Add(tempwarningUI.Class);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh1(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter.SK_SdCharacter'"));
	if(tempMesh1.Succeeded())
	{
		PlayerMesh.Add(tempMesh1.Object);
	}	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh2(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter_WGS.SK_SdCharacter_WGS'"));
	if(tempMesh2.Succeeded())
	{
		PlayerMesh.Add(tempMesh2.Object);
	}	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh3(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter_WTD.SK_SdCharacter_WTD'"));
	if(tempMesh3.Succeeded())
	{
		PlayerMesh.Add(tempMesh3.Object);
	}	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh4(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter_LS_fishingrob.SK_SdCharacter_LS_fishingrob'"));
	if(tempMesh4.Succeeded())
	{
		PlayerMesh.Add(tempMesh4.Object);
	}
}


void ASH_Player::BeginPlay()
{
	Super::BeginPlay();
	MainHUD = CreateWidget<UPlayerMainWG>(GetWorld(), MainWGFactory);
	MainHUD->AddToViewport();
	playerAnim = Cast<USH_PlayerAnim>(GetMesh()->GetAnimInstance());

	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));

	playerCon = GetWorld()->GetFirstPlayerController();
	playerCon->PlayerCameraManager->ViewPitchMin = -30.0f;
	playerCon->PlayerCameraManager->ViewPitchMax = 60.0f;

	dieUI = CreateWidget<UIH_DieUI>(GetWorld(), UIFactory[0]);
	loadingUI = CreateWidget<UIH_LoadingUI>(GetWorld(), UIFactory[1]);
	dialogueUI = CreateWidget<UMainDialogueUI>(GetWorld(), UIFactory[2]);
	warningUI = CreateWidget<UIH_WarningUI>(GetWorld(), UIFactory[3]);
}

void ASH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASH_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent != nullptr)
	{
		EnhancedInputComponent->BindAction(fkey, ETriggerEvent::Triggered, this, &ASH_Player::interactionObject); //F키
		MoveComp->SetupPlayerInputComponent(EnhancedInputComponent);
		InvenComp->SetupPlayerInputComponent(EnhancedInputComponent);
		AttackComp->SetupPlayerInputComponent(EnhancedInputComponent);
		QuestComp->SetupPlayerInputComponent(EnhancedInputComponent);
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
	DrawDebugSphere(GetWorld(), collisionLoc, interColli.GetSphereRadius(), 20, FColor::Red, false, 1,0, 0.5);

	if (bhit)
	{
		
		AItemBase* curritem = Cast<AItemBase>(hitinfo.GetActor());
		if (curritem != nullptr)
		{
			curritem->GetItem();
		}
		
		AInteractiveObjectBase* currobject = Cast<AInteractiveObjectBase>(hitinfo.GetActor());
		if (currobject != nullptr)
		{
			currobject->Interaction();
		}

		ANPCBase* currNPC = Cast<ANPCBase>(hitinfo.GetActor());
		if (currNPC != nullptr)
		{
			if (DataManager->GetCurrQuestInfo(QuestComp->mainQuestIdx).completeNpcIdx == currNPC->idx)
			{
				currNPC->InteractNPC();
			}
			else
			{

			}
		}

		APuzzleGuide* currGuide = Cast<APuzzleGuide>(hitinfo.GetActor());
		if (currGuide != nullptr)
		{
			currGuide->MoveToPuzzleActor();
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
