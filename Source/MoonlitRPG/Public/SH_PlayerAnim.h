// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AttackComponent.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float dirV = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dirH = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAir = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHoldingWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPutInPlay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType currWeapon;

	float currentTime = 0;

	void SwitchCheck(FDamageRange handRange, FDamageRange daggerRange, FDamageRange swordRange);

	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_NextCombo();
	UFUNCTION()
	void AnimNotify_CommonAttack();
	UFUNCTION()
	void AnimNotify_IntensiveAttack1();
	UFUNCTION()
	void AnimNotify_IntensiveAttack2();
	UFUNCTION()
	void AnimNotify_SpecialAttack1();
	UFUNCTION()
	void AnimNotify_SpecialAttack2();
	UFUNCTION()
	void AnimNotify_DashEnd();	
	UFUNCTION()
	void AnimNotify_DashToWalk();
	UFUNCTION()
	void AnimNotify_DamagedEnd();
	UFUNCTION()
	void AnimNotify_DieEnd();
	UFUNCTION()
	void AnimNotify_Put_In_Start();
	UFUNCTION()
	void AnimNotify_Put_In_End();
	UFUNCTION()
	void AnimNotify_Equipped();
};
