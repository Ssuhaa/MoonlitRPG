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
#include <UMG/Public/Components/WidgetComponent.h>
#include "IH_EnemyDamageUI.h"
#include "WidgetActorBase.h"
#include "IH_DamageActor.h"
#include "MainDialogueUI.h"
#include "NPCBase.h"
#include "QuestComponent.h"


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
	
	if(target->bUIOpen || target->dialogueUI->npc != nullptr)
	{
		if (currHP <= 0)
		{
			ChangeState(EEnemyState::Die);
		}
		else
		{ 
			ChangeState(EEnemyState::Idle);
		}

		return;
	}

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
	case EEnemyState::Avoid:
		AvoidState();
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
	float chaseDist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());


	if (returnDist > moveRange)
	{
		ChangeState(EEnemyState::Return);
	}

	if (chaseDist <= avoidRange)
	{
		ChangeState(EEnemyState::Avoid);
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
	FVector direction = target->GetActorLocation() - me->GetActorLocation();
	float dot = FVector::DotProduct(forwardVector, direction.GetSafeNormal());
	float degree = UKismetMathLibrary::DegAcos(dot);
	
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	LookAtSmooth();

	if (distance <= avoidRange && avoidCount < 3)
	{
		ChangeState(EEnemyState::Avoid);
	}

	if (chaseCount > 3 || avoidCount > 3)
	{
		ChangeState(EEnemyState::Attack);
		chaseCount = 0;
		avoidCount = 0;
	}

	float randSec = FMath::RandRange(3, 7);

	if (DelayComplete(randSec))
	{
		if (IsTargetTrace())
		{
			if (degree < 45.0 && distance <= attackRange)
			{
				ChangeState(EEnemyState::Attack);
			}
			else
			{
				ChangeState(EEnemyState::Chase);
			}
		}
		else
		{
			ChangeState(EEnemyState::Move);
		}
	}
}

void UEnemy_FSM::ReceiveDamage(int32 damage)
{
	prevHP = currHP;
	bUpdateHP = true;
	ratioHP = 0;

	currHP -= damage;
	me->enemyHPUI->ReduceHP(currHP, maxHP);

	if (!bDiestart)
	{
		float randLocX = FMath::RandRange(-15, 15);
		FloatingDamage();
		damageUI->UpdateDamage(damage);
		damageUI->FloatingAnimation();
		if (currHP > 0)
		{
			ChangeState(EEnemyState::Damage);
			me->ImpactEffect();
		}
		else
		{
			ChangeState(EEnemyState::Die);
			me->ImpactEffect();
			me->PlayAnimMontage(enemyMontage, 1.0f, FName(TEXT("Die")));
		}
	}
}

void UEnemy_FSM::DamageState()
{
	ChangeState(EEnemyState::AttackDelay);
}

void UEnemy_FSM::AvoidState()
{
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	switch (randomIndex)
	{
		case 0:
		{
			LookAtSmooth();
			me->SetActorLocation(me->GetActorLocation() + me->GetActorForwardVector()*-1);
			if (distance > randomRange)
			{
				ChangeState(EEnemyState::AttackDelay);
			}
			break;
		}
		case 1:
		{
			LookAtSmooth();
			me->SetActorLocation(me->GetActorLocation() + me->GetActorForwardVector() * -1 + me->GetActorRightVector() * -1);
			if (distance > randomRange)
			{
				ChangeState(EEnemyState::AttackDelay);
			}
			break;
		}
		case 2:
		{
			LookAtSmooth();
			me->SetActorLocation(me->GetActorLocation() + me->GetActorForwardVector() * -1 + me->GetActorRightVector());
			if (distance > randomRange)
			{
				ChangeState(EEnemyState::AttackDelay);
			}
			break;
		}
	}
}

void UEnemy_FSM::DieState()
{
	if (bDiedone)
	{
		
		if (DelayComplete(2.0))
		{
			if (me->spawnMoney.IsValidIndex(0))
			{
				int32 randMoney = FMath::RandRange(2,3);
				for (int32 i = 1; i <= randMoney; i++)
				{
					GetWorld()->SpawnActor<AMoney>(me->spawnMoney[0], me->itemSpawnPos->GetComponentLocation(), me->itemSpawnPos->GetComponentRotation());
				}
			}

			if (me->spawnItems.IsValidIndex(0))		// spawnItem 배열의 첫번째 요소가 있을 때
			{
				int32 randAmount = FMath::RandRange(1, 2);
				for (int32 i = 1; i <= randAmount; i++)
				{
					int32 itemNum = FMath::RandRange(0, me->spawnItems.Num()-1);
					GetWorld()->SpawnActor<AItemBase>(me->spawnItems[itemNum], me->itemSpawnPos->GetComponentLocation(), me->itemSpawnPos->GetComponentRotation());
				}
			}

			if (me->Manager != nullptr)
			{
				me->Manager->deathCount++;
				me->SetActive(false);

				if (me->Manager->deathCount == me->Manager->spawnNumber)	// 죽인 횟수가 스폰된 개수와 같으면
				{
					if (target->QuestComp->MainQuest->SubType == ESubQuestType::Hunt)
					{
						target->QuestComp->CheackRequirementTarget(me->Manager->EnemyManagerIdx);
						target->QuestComp->CompleteMainQuest();
					}
					
				}

				me->Manager->enemyArr.Add(me);	// enemy 배열에 마지막에 죽은 enemy를 Add
				me->Manager->canSpawn = true;	// 다시 스폰 가능
				me->Manager->deathCount = 0;	// 죽인 횟수 초기화
			
			}
			

			currHP = maxHP;
			ChangeState(EEnemyState::Idle);
			me->StopAnimMontage(enemyMontage);
			bDiedone = false;
			bDiestart = false;
		}
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

	if (this->IsActive())
	{
		switch (state)
		{
		case EEnemyState::Idle:
			break;

		case EEnemyState::Move:
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			me->compEnemyHP->SetVisibility(false);
			UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
			FNavLocation loc;
			ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
			randPos = loc.Location;
			break;
		}
		case EEnemyState::Chase:
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
			me->compEnemyHP->SetVisibility(true);
			me->compExclamation->SetVisibility(true);
			bAttackEnd = false;
			chaseCount++;
			break;
		}
		case EEnemyState::Attack:
		{
			me->compEnemyHP->SetVisibility(true);
			break;
		}
		case EEnemyState::AttackDelay:
		{
			me->compEnemyHP->SetVisibility(true);
			break;
		}
		case EEnemyState::Avoid:
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 100.0f;
			me->compEnemyHP->SetVisibility(true);
			randomIndex = FMath::RandRange(0, 2);
			randomRange = FMath::RandRange(85, 95);
			avoidCount++;
			break;
		}
		case EEnemyState::Damage:
		{
			me->compEnemyHP->SetVisibility(true);
			int32 randNum = FMath::RandRange(0, 1);
			FString randomSection = FString::Printf(TEXT("Damage%d"), randNum);
			me->PlayAnimMontage(enemyMontage, 1.0f, FName(*randomSection));
// 			FVector direction = target->GetActorLocation() - me->GetActorLocation();
// 			direction.Z = 0;
// 			FRotator rotation = FRotationMatrix::MakeFromX(direction).Rotator();
// 			me->SetActorRotation(rotation);
			break;
		}
		case EEnemyState::Return:
		{
			me->compEnemyHP->SetVisibility(false);
			me->compExclamation->SetVisibility(false);
			break;
		}
		case EEnemyState::Die:
		{
			DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
			me->compEnemyHP->SetVisibility(false);
			bDiestart = true;
			break;
		}
		}
	}
}

bool UEnemy_FSM::IsTargetTrace()
{
	FVector forwardVector = me->GetActorForwardVector();
	FVector direction = target->GetActorLocation() - me->GetActorLocation();
	float dot = FVector::DotProduct(forwardVector, direction.GetSafeNormal());		// ForwardVector는 값이 1이기 때문에 Normalize를 해주지 않아도 됨.
	float degree = UKismetMathLibrary::DegAcos(dot);

	if (degree < 180 && direction.Length() < traceRange)
	{
		FHitResult hitinfo;
		FCollisionShape myCollision = FCollisionShape::MakeSphere(25.0f);
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);

		bool bHit = GetWorld()->SweepSingleByChannel(hitinfo, me->GetActorLocation(), me->GetActorLocation()+ direction, FQuat::Identity, ECC_Visibility, myCollision, param);

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
		if (currState == EEnemyState::Return)
		{
			ChangeState(EEnemyState::Idle);
			currHP = maxHP;
			me->enemyHPUI->UpdateHP(currHP, maxHP);
			me->enemyHPUI->ReduceHP(currHP, maxHP);
		}
		else
		{
			ChangeState(EEnemyState::Idle);
		}
	}
}

void UEnemy_FSM::LookAtSmooth()
{
	if (target != nullptr)
	{
		FVector direction = target->GetActorLocation() - me->GetActorLocation();
		direction.Z = 0;
		FRotator rotation = FRotationMatrix::MakeFromX(direction).Rotator();

		me->SetActorRotation(rotation);
	}
}

bool UEnemy_FSM::PlayerCheck()
{
	FCollisionShape attackCollision = FCollisionShape::MakeSphere(10);
	FCollisionQueryParams param;
	param.AddIgnoredActor(me);
	FHitResult hitinfo;

	bool bHit = GetWorld()->SweepSingleByChannel(hitinfo, me->GetActorLocation(), me->GetActorLocation()+me->GetActorForwardVector()*100, FQuat::Identity,
	ECC_Visibility, attackCollision, param);

	if (bHit)
	{
		if (hitinfo.GetActor()->GetName().Contains(TEXT("Player")))
		{
			enemyDamage = FMath::RandRange(minDamage, maxDamage);
			return true;
		}
	}
	return false;
}

void UEnemy_FSM::FloatingDamage()
{
	float randLocX = FMath::RandRange(-15, 15);
	damageUI = GetWorld()->SpawnActor<AIH_DamageActor>(damageActor, me->itemSpawnPos->GetComponentLocation() + me->itemSpawnPos->GetRightVector() * randLocX, me->itemSpawnPos->GetComponentRotation());
}