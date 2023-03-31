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
	Avoid,
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

	UPROPERTY()
	class ASH_Player* target;
	UPROPERTY()
	class AEnemyBase* me;
	UPROPERTY()
	class UIH_EnemyAnim* anim;
	UPROPERTY()
	class AAIController* ai;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class AIH_DamageActor> damageActor;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class AIH_DamageActor* damageUI;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* enemyMontage;

	float currentTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	float traceRange = 700.0f;		// 인지 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	float avoidRange = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	float randomRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	int32 randomIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	float attackRange = 135.0f;		// 공격 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	float moveRange = 2000.0f;		// 추격 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HP)
	float maxHP = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minDamage = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxDamage = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerHP)
	float enemyDamage = 5;

	float currHP;
	float prevHP;
	float ratioHP;

	bool bUpdateHP = false;		// HP Bar 업데이트
	bool bDiestart = false;		// 죽음 시작 확인(true면 공격할 수 없음)
	bool bDiedone = false;		// 죽음 끝 확인(true면 Enemy 소멸)
	bool bAttackEnd = false;	// 공격 끝 확인(true면 움직임 가능)

	FVector originPos;		// 최초 위치
	FVector randPos;		// 랜덤 위치

	int32 chaseCount = 0;
	int32 avoidCount = 0;

public:
	void IdleState();	// 대기 상태
	void MoveState();	// 이동 상태
	void ChaseState();	// 쫓는 상태
	void ReturnState();		// 복귀 상태
	void AttackState();		// 공격 상태
	void AttackDelayState();	// 공격대기 상태
	void DamageState();		// 피격 상태
	void AvoidState();
	void DieState();	// 죽음 상태

	void ReceiveDamage(int32 damage);
	bool DelayComplete(float delayTime);
	void ChangeState(EEnemyState state);
	bool IsTargetTrace();
	void MoveToPos(FVector pos);
	void LookAtSmooth();
	bool PlayerCheck();
	void FloatingDamage();

	UPROPERTY()
	class ADataManager* DataManager;

};
