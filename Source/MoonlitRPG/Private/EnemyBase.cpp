// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Enemy_FSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "IH_EnemyHPUI.h"
#include <Components/CapsuleComponent.h>
#include "ItemBase.h"
#include "IH_EnemyDamageUI.h"
#include "IH_ExclamationUI.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	FSM = CreateDefaultSubobject<UEnemy_FSM>(TEXT("FSM"));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FClassFinder<UAnimInstance>tempanim(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrint/ABP_IH_Enemy.ABP_IH_Enemy_C'"));
	if (tempanim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempanim.Class);
	}

	compEnemyHP = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Widget Component"));
	compEnemyHP->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UIH_EnemyHPUI>tempEnemyHP(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_EnemyHP.WG_EnemyHP_C'"));
	if(tempEnemyHP.Succeeded())
	{
		compEnemyHP->SetWidgetClass(tempEnemyHP.Class);
		compEnemyHP->SetWidgetSpace(EWidgetSpace::Screen);
		compEnemyHP->SetRelativeLocation(FVector(0, 0, 50));
		compEnemyHP->SetVisibility(false);
	}

	itemSpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("Item Spawn Position"));
	itemSpawnPos->SetupAttachment(RootComponent);
	itemSpawnPos->SetRelativeLocation(FVector(15, 0, 40));

	compExclamation = CreateDefaultSubobject<UWidgetComponent>(TEXT("Exclamation Widget Component"));
	compExclamation->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UIH_ExclamationUI>tempExcla(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Exclamation.WG_Exclamation_C'"));
	if (tempExcla.Succeeded())
	{
		compExclamation->SetWidgetClass(tempExcla.Class);
		compExclamation->SetWidgetSpace(EWidgetSpace::Screen);
		compExclamation->SetRelativeLocation(FVector(10, 0, 35));
		compExclamation->SetVisibility(false);
	}
	
	bUseControllerRotationYaw = false;
 	GetCharacterMovement()->bUseControllerDesiredRotation = true;
 	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Enemy")));
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	enemyHPUI = Cast<UIH_EnemyHPUI>(compEnemyHP->GetUserWidgetObject());
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::SetActive(bool bActive)
{
	if (bActive)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		FSM->originPos = GetActorLocation();
	}
	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	GetMesh()->SetActive(bActive);
	GetMesh()->SetVisibility(bActive);
	GetMovementComponent()->SetActive(bActive);
	FSM->SetActive(bActive);
}