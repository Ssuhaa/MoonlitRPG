// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Chase,
	Attack,
	AttackDelay,
	Damage,
	Return,
	Die
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOONLITRPG_API UEnemy_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FSM")
	EEnemyState currState = EEnemyState::Idle;

	class AIH_Enemy* me;

	float currentTime = 0;

public:
	void IdleState(); // ��� ����
	void MoveState(); // �̵� ����
	void ChaseState(); // �Ѵ� ����
	void AttackState(); // ���� ����
	void DamageState(); // �ǰ� ����
	void DieState(); // ���� ����

	bool DelayComplete(float delayTime);
	void ChangeState(EEnemyState state);
};
