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
	class AIH_Enemy* me;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	EEnemyState animState;

public:
	UFUNCTION()
	void AnimNotify_DieEnd();
};
