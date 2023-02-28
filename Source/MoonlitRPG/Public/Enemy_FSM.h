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

	class ASH_Player* target;
	class AIH_Enemy* me;
	class UIH_EnemyAnim* anim;

	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	float currentTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float traceRange = 700.0f;		// 인지 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackRange = 100.0f;		// 공격 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveRange = 2000.0f;		// 추격 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxHP = 10;

	float currHP;

	FVector originPos;		// 최초 위치
	FVector randPos;		// 랜덤 위치

public:
	void IdleState();	// 대기 상태
	void MoveState();	// 이동 상태
	void ChaseState();	// 쫓는 상태
	void ReturnState();		// 복귀 상태
	void AttackState();		// 공격 상태
	void AttackDelayState();	// 공격대기 상태
	void DamageState();		// 피격 상태
	void DieState();	// 죽음 상태

	void ReceiveDamage(float damage);
	bool DelayComplete(float delayTime);
	void ChangeState(EEnemyState state);
	bool IsTargetTrace();
	void MoveToPos(FVector pos);
};
