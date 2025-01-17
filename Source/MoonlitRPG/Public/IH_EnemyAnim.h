// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy_FSM.h"
#include "IH_EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UIH_EnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override;

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	class APawn* ownerPawn;
	class AEnemyBase* me;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	EEnemyState animState;

public:
	UFUNCTION()
	void AnimNotify_EnemyDieEnd();
	UFUNCTION()
	void AnimNotify_EnemyAttack();
	UFUNCTION()
	void AnimNotify_EnemyAttackEnd();
};
