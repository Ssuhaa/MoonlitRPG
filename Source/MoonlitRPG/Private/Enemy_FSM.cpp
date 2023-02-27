// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_FSM.h"
#include "IH_Enemy.h"

// Sets default values for this component's properties
UEnemy_FSM::UEnemy_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemy_FSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AIH_Enemy>(GetOwner());
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

	case EEnemyState::Attack:
		AttackState();
		break;

	case EEnemyState::Damage:
		DamageState();
		break;

	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UEnemy_FSM::IdleState()
{
	if (DelayComplete(3.0))
	{
		ChangeState(EEnemyState::Move);
	}
}

void UEnemy_FSM::MoveState()
{

}

void UEnemy_FSM::ChaseState()
{

}

void UEnemy_FSM::AttackState()
{

}

void UEnemy_FSM::DamageState()
{

}

void UEnemy_FSM::DieState()
{

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
	currentTime = 0;
	currState = state;

	switch (state)
	{
	case EEnemyState::Idle:
		UE_LOG(LogTemp, Warning, TEXT("Idle"));
		IdleState();
		break;

	case EEnemyState::Move:
		UE_LOG(LogTemp, Warning, TEXT("Move"));
		MoveState();
		break;

	case EEnemyState::Chase:
		UE_LOG(LogTemp, Warning, TEXT("Chase"));
		ChaseState();
		break;

	case EEnemyState::Attack:
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		AttackState();
		break;

	case EEnemyState::Damage:
		UE_LOG(LogTemp, Warning, TEXT("Damage"));
		DamageState();
		break;

	case EEnemyState::Die:
		UE_LOG(LogTemp, Warning, TEXT("Die"));
		DieState();
		break;
	}
}