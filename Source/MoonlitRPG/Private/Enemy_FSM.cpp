// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_FSM.h"
#include "EnemyBase.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <NavigationSystem/Public/NavigationSystem.h>
#include <AIController.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "IH_EnemyAnim.h"
#include "IH_EnemyHPUI.h"
#include "ItemBase.h"
#include "IH_EnemyManager.h"
#include <Components/CapsuleComponent.h>


// Sets default values for this component's properties
UEnemy_FSM::UEnemy_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Animations/Montage/AMT_Enemy.AMT_Enemy'"));
	if (tempMontage.Succeeded())
	{
		enemyMontage = tempMontage.Object;
	}

	bAutoActivate = true;
}


// Called when the game starts
void UEnemy_FSM::BeginPlay()
{
	Super::BeginPlay();

	target = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));

	me = Cast<AEnemyBase>(GetOwner());

	ai = Cast<AAIController>(me->GetController());

	anim = Cast<UIH_EnemyAnim>(me->GetMesh()->GetAnimInstance());
	
	originPos = me->GetActorLocation();

	currHP = maxHP;
}


// Called every frame
void UEnemy_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Chase:
		ChaseState();
		break;
	case EEnemyState::Return:
		ReturnState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::AttackDelay:
		AttackDelayState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}

	if (bUpdateHP)
	{
		ratioHP += DeltaTime;
		
		if (ratioHP >= 1)
		{
			ratioHP = 1;
			bUpdateHP = false;
		}

		float lerpHP = FMath::Lerp(prevHP, currHP, ratioHP);
		me->enemyHPUI->UpdateHP(lerpHP, maxHP);
	}
}

void UEnemy_FSM::IdleState()
{
	if(IsTargetTrace())
	{
		ChangeState(EEnemyState::Chase);
	}
	else
	{
		if (DelayComplete(3.0))
		{
			ChangeState(EEnemyState::Move);
		}
	}
}

void UEnemy_FSM::MoveState()
{
	MoveToPos(randPos);

	if (IsTargetTrace())
	{
		ChangeState(EEnemyState::Chase);
	}
}

void UEnemy_FSM::ChaseState()
{
	float returnDist = FVector::Distance(originPos, target->GetActorLocation());
	float attackDist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	if (returnDist > moveRange)
	{
		ChangeState(EEnemyState::Return);
	}

	if (attackDist < attackRange)
	{
		ChangeState(EEnemyState::Attack);
	}
	else
	{
		ai->MoveToLocation(target->GetActorLocation());
	}
}

void UEnemy_FSM::ReturnState()
{
	MoveToPos(originPos);
}

void UEnemy_FSM::AttackState()
{
	ChangeState(EEnemyState::AttackDelay);
}

void UEnemy_FSM::AttackDelayState()
{
	FVector forwardVector = me->GetActorForwardVector();
	FVector targetVector = target->GetActorLocation() - me->GetActorLocation();
	float dot = FVector::DotProduct(forwardVector, targetVector.GetSafeNormal());
	float degree = UKismetMathLibrary::DegAcos(dot);
	
	float chaseDist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	if (DelayComplete(2.0))
	{
		if (degree < 45.0)
		{
			ChangeState(EEnemyState::Attack);
		}
		else
		{
			ChangeState(EEnemyState::Idle);
		}
	}

	if (chaseDist > attackRange)
	{
		ChangeState(EEnemyState::Chase);
	}
}

void UEnemy_FSM::DamageState()
{
	if (DelayComplete(1.0))
	{
		ChangeState(EEnemyState::Idle);
	}
}

void UEnemy_FSM::DieState()
{
	if (bDiedone)
	{
		if (DelayComplete(2.0))
		{
			if (me->spawnItems.IsValidIndex(0))		// spawnItem 배열의 첫번째 요소가 있을 때
			{
				int32 randAmount = FMath::RandRange(1, 3);
				for (int32 i = 1; i <= randAmount; i++)
				{
					int32 itemNum = FMath::RandRange(0, me->spawnItems.Num()-1);
					GetWorld()->SpawnActor<AItemBase>(me->spawnItems[itemNum], me->itemSpawnPos->GetComponentLocation(), me->itemSpawnPos->GetComponentRotation());
				}
			}

			AIH_EnemyManager* manager = Cast<AIH_EnemyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AIH_EnemyManager::StaticClass()));
			manager->enemyArr.Add(me);

			me->SetActive(false);

			currHP = maxHP;
			ChangeState(EEnemyState::Idle);
			me->StopAnimMontage(enemyMontage);
			bDiedone = false;
		}
	}
}

void UEnemy_FSM::ReceiveDamage(float damage)
{
	prevHP = currHP;
	bUpdateHP = true;
	ratioHP = 0;

	currHP -= damage;
	me->enemyHPUI->ReduceHP(currHP, maxHP);

	if (currHP > 0)
	{
		ChangeState(EEnemyState::Damage);
	}
	else
	{
		ChangeState(EEnemyState::Die);
		me->PlayAnimMontage(enemyMontage, 1.0f, FName(TEXT("Die")));
	}
}

bool UEnemy_FSM::DelayComplete(float delayTime)
{
	currentTime += GetWorld()->GetDeltaSeconds();

	if (currentTime > delayTime)
	{
		currentTime = 0;
		return true;
	}
	else return false;
}

void UEnemy_FSM::ChangeState(EEnemyState state)
{
	currState = state;
	anim->animState=state;
	currentTime = 0;
	ai->StopMovement();

	switch (state)
	{
	case EEnemyState::Idle:
		break;

	case EEnemyState::Move:
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
		break;
	}

	case EEnemyState::Chase:
		me->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		break;

	case EEnemyState::Attack:
		break;

	case EEnemyState::Damage:
	{
		int32 randNum = FMath::RandRange(0, 1);
		FString randomSection = FString::Printf(TEXT("Damage%d"), randNum);
		me->PlayAnimMontage(enemyMontage, 1.0f, FName(*randomSection));
		break;
	}

	case EEnemyState::Die:
//		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

bool UEnemy_FSM::IsTargetTrace()
{
	FVector forwardVector = me->GetActorForwardVector();
	FVector targetVector = target->GetActorLocation() - me->GetActorLocation();
	float dot = FVector::DotProduct(forwardVector, targetVector.GetSafeNormal());		// ForwardVector는 값이 1이기 때문에 Normalize를 해주지 않아도 됨.
	float degree = UKismetMathLibrary::DegAcos(dot);

	if (degree < 70 && targetVector.Length() < traceRange)
	{
		FHitResult hitinfo;
		FCollisionShape myCollision = FCollisionShape::MakeSphere(25.0f);
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);

		bool bHit = GetWorld()->SweepSingleByChannel(hitinfo, me->GetActorLocation(), me->GetActorLocation()+targetVector, FQuat::Identity, ECC_Visibility, myCollision, param);

// 		bool bHit = GetWorld()->LineTraceSingleByChannel(hitinfo, me->GetActorLocation(), target->GetActorLocation(), ECC_Visibility, param);
// 		DrawDebugLine(GetWorld(), me->GetActorLocation(), target->GetActorLocation(), FColor::Cyan, false, 0.5f, 2.0f);

		if (bHit)
		{
			if (hitinfo.GetActor()->GetName().Contains(TEXT("Player")))
			{
				return true;
			}
		}
	}

	return false;
}

void UEnemy_FSM::MoveToPos(FVector pos)
{
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		ChangeState(EEnemyState::Idle);
	}
}