// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SH_PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API USH_PlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY()
	class ASH_Player* Player;


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float dirV = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dirH = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAir = false;
};
