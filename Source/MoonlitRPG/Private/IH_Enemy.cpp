// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_Enemy.h"
#include "Enemy_FSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "IH_EnemyHPUI.h"

AIH_Enemy::AIH_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	FSM = CreateDefaultSubobject<UEnemy_FSM>(TEXT("FSM"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempmesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Geometry/JumpingSeed_Character/SM_PlayerMesh1.SM_PlayerMesh1'"));
	if (tempmesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempmesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance>tempanim(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrint/ABP_IH_Enemy.ABP_IH_Enemy_C'"));
	if (tempanim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempanim.Class);
	}

	compEnemyHP = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Widget Component"));
	compEnemyHP->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UIH_EnemyHPUI>tempEnemyHP(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/WG_EnemyHP.WG_EnemyHP_C'"));
	if(tempEnemyHP.Succeeded())
	{
		compEnemyHP->SetWidgetClass(tempEnemyHP.Class);
		compEnemyHP->SetWidgetSpace(EWidgetSpace::Screen);
		compEnemyHP->SetRelativeLocation(FVector(90, 0, 0));
	}
	
	bUseControllerRotationYaw = false;
 	GetCharacterMovement()->bUseControllerDesiredRotation = true;
 	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void AIH_Enemy::BeginPlay()
{
	Super::BeginPlay();

	enemyHPUI = Cast<UIH_EnemyHPUI>(compEnemyHP->GetWidgetClass());
}

void AIH_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}