// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOONLITRPG_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TArray<class UInputAction*> inputarray;
	TArray<FHitResult>hitinfos;

	void CommonAttack();
	void intensiveAttack();
	void SpecialAttack();
	bool CanAttack(float attackRadius, float attackLength);

	class ASH_Player* player;
	class AEnemyBase* Target;
	class AHitObjectBase* HitObject;

public:
	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* attackMontage;
	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* combo1Montage;
	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* combo2Montage;

	bool isAttacking = false;
	bool goToNextCombo = false;
	bool coolTimeRunning = false;

	int32 attackCount = 0;

	UPROPERTY(EditAnywhere)
	float addPercent;

	float specialCount = 0;

	UPROPERTY(EditAnywhere)
	float intensiveDelay = 5;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);

	void NextCombo();
	void TargetCheck(float attackRadius, float attackLength, float damage, float pushForce);
};
