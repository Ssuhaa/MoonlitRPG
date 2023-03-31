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
	float traceRange = 700.0f;		// ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	float avoidRange = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	float randomRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	int32 randomIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	float attackRange = 135.0f;		// ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
	float moveRange = 2000.0f;		// �߰� ����

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

	bool bUpdateHP = false;		// HP Bar ������Ʈ
	bool bDiestart = false;		// ���� ���� Ȯ��(true�� ������ �� ����)
	bool bDiedone = false;		// ���� �� Ȯ��(true�� Enemy �Ҹ�)
	bool bAttackEnd = false;	// ���� �� Ȯ��(true�� ������ ����)

	FVector originPos;		// ���� ��ġ
	FVector randPos;		// ���� ��ġ

	int32 chaseCount = 0;
	int32 avoidCount = 0;

public:
	void IdleState();	// ��� ����
	void MoveState();	// �̵� ����
	void ChaseState();	// �Ѵ� ����
	void ReturnState();		// ���� ����
	void AttackState();		// ���� ����
	void AttackDelayState();	// ���ݴ�� ����
	void DamageState();		// �ǰ� ����
	void AvoidState();
	void DieState();	// ���� ����

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
