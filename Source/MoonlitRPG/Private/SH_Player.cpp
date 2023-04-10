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
#include <Components/PostProcessComponent.h>
#include <Camera/CameraActor.h>
#include <Kismet/KismetMathLibrary.h>
#include <../Plugins/MovieScene/ActorSequence/Source/ActorSequence/Public/ActorSequenceComponent.h>
#include <MovieScene/Public/MovieSceneSequencePlayer.h>
#include <../Plugins/MovieScene/ActorSequence/Source/ActorSequence/Public/ActorSequencePlayer.h>
#include <Sound/SoundCue.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>


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

	// 손, 등에 붙는 스태틱메시 컴포넌트
	EquippedComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp"));
	EquippedComp1->SetupAttachment(GetMesh(), TEXT("Weapon_Socket_1"));
	EquippedComp1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EquippedComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp2"));
	EquippedComp2->SetupAttachment(GetMesh(), TEXT("Weapon_Socket_2"));
	EquippedComp2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GrabComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrabComp1"));
	GrabComp1->SetupAttachment(GetMesh(), TEXT("GrabSocket_R"));
	GrabComp1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GrabComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrabComp2"));
	GrabComp2->SetupAttachment(GetMesh(), TEXT("GrabSocket_L"));
	GrabComp2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GrabComp3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrabComp3"));
	GrabComp3->SetupAttachment(GetMesh(), TEXT("TwoHandSocket"));
	GrabComp3->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// PostProcess
	PlayerPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Player Post Process Component"));
	PlayerPostProcess->SetupAttachment(RootComponent);

	// 스킬 시퀀스 카메라
	SequenceCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Sequence Camera"));
	SequenceCamComp->SetupAttachment(RootComponent);
	SequenceChildComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Skill Sequence Child"));
	SequenceChildComp->SetupAttachment(SequenceCamComp);

	// 대화창 카메라
	DialogueCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Dialogue Camera"));
	DialogueCamComp->SetupAttachment(SpringArmComp);
	DialogueChildComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("DialogueCam Child"));
	DialogueChildComp->SetupAttachment(DialogueCamComp);

	InvenComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InvenComp"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComp"));
	QuestComp = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComp"));

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempHit(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Impact_2.P_Impact_2'"));
	if (tempHit.Succeeded())
	{
		playerParticle.Add(tempHit.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempCharge(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Lightning_2.P_Lightning_2'"));
	if (tempCharge.Succeeded())
	{
		playerParticle.Add(tempCharge.Object);
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

	ConstructorHelpers::FObjectFinder<USoundCue> tempImpactSound1(TEXT("/Script/Engine.SoundCue'/Game/Sound/SFX/sc_Impact.sc_Impact'"));
	if (tempImpactSound1.Succeeded())
	{
		SoundArr.Add(tempImpactSound1.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundCue> tempImpactSound2(TEXT("/Script/Engine.SoundCue'/Game/Sound/SFX/sc_LargeImpact.sc_LargeImpact'"));
	if (tempImpactSound2.Succeeded())
	{
		SoundArr.Add(tempImpactSound2.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempClickSound1(TEXT("/Script/Engine.SoundWave'/Game/Sound/SFX/FX_Click_1.FX_Click_1'"));
	if (tempClickSound1.Succeeded())
	{
		SoundArr.Add(tempClickSound1.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempClickSound2(TEXT("/Script/Engine.SoundWave'/Game/Sound/SFX/FX_Click_2.FX_Click_2'"));
	if (tempClickSound2.Succeeded())
	{
		SoundArr.Add(tempClickSound2.Object);
	}

	SkeletalMeshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter_WGS.SK_SdCharacter_WGS'"));
	SkeletalMeshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter_WTD.SK_SdCharacter_WTD'"));
	SkeletalMeshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter_LS_fishingrob.SK_SdCharacter_LS_fishingrob'"));
	SkeletalMeshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter.SK_SdCharacter'"));

	dieUI = CreateWGClass<UIH_DieUI>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Die.WG_Die_C'"));
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
	
	SkillSequenceComp = Cast<UActorSequenceComponent>(GetComponentByClass(UActorSequenceComponent::StaticClass()));
	skillPlay = SkillSequenceComp->GetSequencePlayer();
	skillPlay->OnFinished.AddDynamic(this, &ASH_Player::EndSkillSequence);

	MainHUD->AddToViewport();
	playerAnim = Cast<USH_PlayerAnim>(GetMesh()->GetAnimInstance());

	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));

	playerCon = GetWorld()->GetFirstPlayerController();
	playerCon->PlayerCameraManager->ViewPitchMin = -50.0f;
	playerCon->PlayerCameraManager->ViewPitchMax = 0.0f;

	PlayerPostProcess->Settings.VignetteIntensity = 0.5f;
	PlayerPostProcess->Settings.MotionBlurAmount = 1.0f;
	PlayerPostProcess->Settings.MotionBlurMax = 50.0f;
	PlayerPostProcess->Settings.BloomIntensity = 4.0f;
}

void ASH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttackComp->hitAnything)
	{
		float randomShake = FMath::RandRange(-3.0f, 3.0f);
		float specialShake = FMath::RandRange(-5.0f, 5.0f);

 		currTime += DeltaTime;
		
		if (AttackComp->isSpecialAttacking)
		{
			CameraShake(DeltaTime, 0.5f, specialShake);
		}
		else
		{
			CameraShake(DeltaTime, 0.2f, randomShake);
		}
	}

	ActiveVignette(DeltaTime);

// 	// 대화했을 때 Zoom In/Out
// 	if (bTalking)
// 	{
// 		zoomIn -= DeltaTime*40;
// 
// 		zoomIn = FMath::Clamp(zoomIn, 75, 90);
// 		CamComp->SetFieldOfView(zoomIn);
// 	}
// 	else
// 	{
// 		zoomIn += DeltaTime*40;
// 
// 		zoomIn = FMath::Clamp(zoomIn, 75, 90);
// 		CamComp->SetFieldOfView(zoomIn);
// 	}
	
	// 대쉬했을 때 Zoom In/Out
	if (MoveComp->bDash)
	{
		dashZoom += DeltaTime*50;

		dashZoom = FMath::Clamp(dashZoom, 90, 100);
		CamComp->SetFieldOfView(dashZoom);
	}
	else
	{
		dashZoom -= DeltaTime*50;

		dashZoom = FMath::Clamp(dashZoom, 90, 100);
		CamComp->SetFieldOfView(dashZoom);
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

void ASH_Player::CameraShake(float deltaTime, float delayTime, float shakeTime)
{
	currTime += deltaTime;

	if (currTime < delayTime)
	{
		CamComp->SetWorldLocation(CamComp->GetComponentLocation() + FVector(shakeTime));
	}
	else
	{
		currTime = 0;
		AttackComp->hitAnything = false;
		CamComp->SetRelativeLocation(FVector(-200, 0, 50));
	}
}

void ASH_Player::ActiveVignette(float deltaTime)
{
	if (AttackComp->isSpecialAttacking)
	{
		PlayerPostProcess->Settings.bOverride_VignetteIntensity = true;
		vignetteValue += deltaTime * 5;
		vignetteValue = FMath::Clamp(vignetteValue, 0.0f, 0.8f);

		PlayerPostProcess->Settings.VignetteIntensity = vignetteValue;
	}
	else
	{
		vignetteValue -= deltaTime * 5;
		vignetteValue = FMath::Clamp(vignetteValue, 0.0f, 0.8f);

		if (vignetteValue <= 0.5)
		{
			PlayerPostProcess->Settings.bOverride_VignetteIntensity = false;
		}

		PlayerPostProcess->Settings.VignetteIntensity = vignetteValue;
	}
}

void ASH_Player::ActiveBlur(bool Active)
{
	if (Active)
	{
		PlayerPostProcess->Settings.bOverride_MotionBlurAmount = true;
		PlayerPostProcess->Settings.bOverride_MotionBlurMax = true;

		PlayerPostProcess->Settings.bOverride_BloomMethod = true;
		PlayerPostProcess->Settings.bOverride_BloomIntensity = true;
	}
	else
	{
		PlayerPostProcess->Settings.bOverride_MotionBlurAmount = false;
		PlayerPostProcess->Settings.bOverride_MotionBlurMax = false;

		PlayerPostProcess->Settings.bOverride_BloomMethod = false;
		PlayerPostProcess->Settings.bOverride_BloomIntensity = false;
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
		UGameplayStatics::PlaySound2D(GetWorld(), SoundArr[2]);
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
			//SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(currNPC->GetActorLocation() - GetActorLocation(), FVector::UpVector));
			playerCon->SetViewTargetWithBlend(DialogueChildComp->GetChildActor(), 0.5f, VTBlend_EaseInOut, 1.0f);
			currNPC->InteractNPC();
			bTalking = true;
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
		if (PlayercurrHP > 0)
		{
			int32 randNum = FMath::RandRange(0, 1);
			FString sectionName = FString::Printf(TEXT("Damaged%d"), randNum);
			PlayAnimMontage(AttackComp->damagedMontage, 1.0f, FName(*sectionName));

			FloatingPlayerDamage();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundArr[0], GetActorLocation());
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
		SetActorLocation(FVector(-80404.0, -59745.0, 1150.0));
		SetActorRotation(FRotator(0, 25, 0));
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


void ASH_Player::SwitchWeaponPos()		
{
	if(currEquipWeapon.Weaponinfo == nullptr) return;
	if (playerAnim->bHoldingWeapon)  // 무기를 휘둘렀으면
	{
		ClearEquipWeapon();
		switch (currEquipWeapon.Weaponinfo->WeaponType)  // 무기가 등에서 손으로 이동
		{
			case EWeaponType::Dagger:
			{
				GrabComp1->SetStaticMesh(currEquipWeapon.Weaponinfo->EquipMesh);
				GrabComp2->SetStaticMesh(currEquipWeapon.Weaponinfo->EquipMesh);
				GrabComp3->SetStaticMesh(nullptr);
				break;
			}
			case EWeaponType::Sword:
			{
				GrabComp1->SetStaticMesh(nullptr);
				GrabComp2->SetStaticMesh(nullptr);
				GrabComp3->SetStaticMesh(currEquipWeapon.Weaponinfo->EquipMesh);
				break;
			}
		}
	}
	else
	{
		ClearGrabWeapon();
		switch (currEquipWeapon.Weaponinfo->WeaponType)  // 무기가 손에서 등으로 이동
		{
			case EWeaponType::Dagger:
			{
				EquippedComp1->SetStaticMesh(currEquipWeapon.Weaponinfo->EquipMesh);
				EquippedComp2->SetStaticMesh(currEquipWeapon.Weaponinfo->EquipMesh);
				break;
			}
			case EWeaponType::Sword:
			{
				EquippedComp1->SetStaticMesh(currEquipWeapon.Weaponinfo->EquipMesh);
				EquippedComp2->SetStaticMesh(nullptr);
				break;
			}
		}
	}
}

void ASH_Player::ClearEquipWeapon()
{
	EquippedComp1->SetStaticMesh(nullptr);
	EquippedComp2->SetStaticMesh(nullptr);
}

void ASH_Player::ClearGrabWeapon()
{
	GrabComp1->SetStaticMesh(nullptr);
	GrabComp2->SetStaticMesh(nullptr);
	GrabComp3->SetStaticMesh(nullptr);
}

void ASH_Player::FadeInOut(bool fadeIn)
{
	if (fadeIn)
	{
		MainHUD->SetVisibility(ESlateVisibility::Hidden);
		GetCharacterMovement()->DisableMovement();
	}
	else
	{
		MainHUD->SetVisibility(ESlateVisibility::Visible);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void ASH_Player::DangSanLevelUp(int32 PlusStamina)
{
	DansanLevel++;
	MoveComp->maxStamina += PlusStamina;
}

void ASH_Player::PlaySkillSequence()
{
	playerCon->SetViewTargetWithBlend(SequenceChildComp->GetChildActor(), 0.0f, VTBlend_EaseInOut, 1.0f);
	chargeComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), playerParticle[1], GetActorLocation()-GetActorUpVector()*30);
	chargeComp->SetRelativeScale3D(FVector(0.5f));

	if (!playerAnim->bHoldingWeapon)
	{
		playerAnim->bHoldingWeapon = true;
		SwitchWeaponPos();
	}

 	skillPlay->Play();
}

void ASH_Player::EndSkillSequence()
{
	chargeComp->DestroyComponent();
	AttackComp->SkillMontStart();
}