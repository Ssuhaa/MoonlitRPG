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
#include <Particles/ParticleSystemComponent.h>
#include "SH_Player.h"
#include "AttackComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include <Kismet/KismetMaterialLibrary.h>
#include "Kismet/GameplayStatics.h"
#include <Sound/SoundCue.h>


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

	hitImpact=CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Hit Impact"));
	hitImpact->SetupAttachment(RootComponent);
	hitImpact->SetRelativeLocation(FVector(30, 0, -20));
	hitImpact->SetRelativeScale3D(FVector(1.0));
	hitImpact->SetActive(false);

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempHit(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Impact_2.P_Impact_2'"));
	if (tempHit.Succeeded())
	{
		impactEffectArr.Add(tempHit.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempCritical(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Impact_1.P_Impact_1'"));
	if (tempCritical.Succeeded())
	{
		impactEffectArr.Add(tempCritical.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempplayerHit(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Impact_4.P_Impact_4'"));
	if (tempplayerHit.Succeeded())
	{
		impactEffectArr.Add(tempplayerHit.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundCue>tempSound(TEXT("/Script/Engine.SoundCue'/Game/Sound/SFX/sc_Impact.sc_Impact'"));
	if (tempSound.Succeeded())
	{
		damagedSound.Add(tempSound.Object);
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	enemyHPUI = Cast<UIH_EnemyHPUI>(compEnemyHP->GetUserWidgetObject());

	for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		instArr.Add(UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(i), nullptr));
		GetMesh()->SetMaterial(i, instArr[i]);
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FSM->bDiestart)
	{
		currTime += DeltaTime;

		if (currTime > 1.5)
		{
			dissolveValue -=DeltaTime*0.35;

			for (int32 i = 0; i < instArr.Num(); i++)
			{
				instArr[i]->SetScalarParameterValue(TEXT("Dead"), dissolveValue);
			}
		}
	}
}

void AEnemyBase::SetActive(bool bActive)
{
	if (bActive)
	{
		dissolveValue = 1;
		for (int32 i = 0; i < instArr.Num(); i++)
		{
			instArr[i]->SetScalarParameterValue(TEXT("Dead"), dissolveValue);
		}

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

void AEnemyBase::ImpactEffect()
{
	if (FSM->target->AttackComp->iscriticAttack)
	{
		hitImpact->SetTemplate(impactEffectArr[1]);
		hitImpact->SetRelativeScale3D(FVector(1.2));
		hitImpact->SetActive(true);
	}
	else
	{
		hitImpact->SetTemplate(impactEffectArr[0]);
		hitImpact->SetActive(true);
	}
}

void AEnemyBase::RandomHitImpact(FVector loc)
{
	UParticleSystemComponent* playerhit = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffectArr[2], loc);
	playerhit->SetRelativeScale3D(FVector(1.0));
}

void AEnemyBase::PlayImpactSound()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), damagedSound[0], GetActorLocation());
}