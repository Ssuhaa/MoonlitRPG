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

ASH_Player::ASH_Player()
{
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
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
		dieUIFactory = tempdieUI.Class;
	}
	ConstructorHelpers::FClassFinder<UIH_LoadingUI> temploadingUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Loading.WG_Loading_C'"));
	if (temploadingUI.Succeeded())
	{
		loadingUIFactory = temploadingUI.Class;
	}
}

void ASH_Player::BeginPlay()
{
	Super::BeginPlay();
	MainHUD = CreateWidget<UPlayerMainWG>(GetWorld(), MainWGFactory);
	MainHUD->AddToViewport();
	playerAnim = Cast<USH_PlayerAnim>(GetMesh()->GetAnimInstance());


	playerCon = GetWorld()->GetFirstPlayerController();
	playerCon->PlayerCameraManager->ViewPitchMin = -30.0f;
	playerCon->PlayerCameraManager->ViewPitchMax = 60.0f;

	dieUI = CreateWidget<UIH_DieUI>(GetWorld(), dieUIFactory);
	loadingUI = CreateWidget<UIH_LoadingUI>(GetWorld(), loadingUIFactory);
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
		EnhancedInputComponent->BindAction(fkey, ETriggerEvent::Triggered, this, &ASH_Player::interactionObject);
		MoveComp->SetupPlayerInputComponent(EnhancedInputComponent);
		InvenComp->SetupPlayerInputComponent(EnhancedInputComponent);
		AttackComp->SetupPlayerInputComponent(EnhancedInputComponent);
	}
}


void ASH_Player::interactionObject()
{
	UE_LOG(LogTemp, Warning, TEXT("FKey"));
	FHitResult hitinfo;
	FCollisionShape interColli = FCollisionShape::MakeSphere(150.0f);
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);
	bool bhit = GetWorld()->SweepSingleByChannel(hitinfo, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_Visibility, interColli, param);
	DrawDebugSphere(GetWorld(), GetActorLocation(), interColli.GetSphereRadius(), 100, FColor::Red, false, 1,0, 0.5);
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
			PlayAnimMontage(AttackComp->playerMontage, 1.0f, FName(*sectionName));
			GetCharacterMovement()->DisableMovement();
		}
		else // 플레이어 죽음
		{
			PlayAnimMontage(AttackComp->playerMontage, 1.0f, FName(TEXT("Die")));
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
		SetActorLocation(warpPoint->compSpawnPos->GetComponentLocation());
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
