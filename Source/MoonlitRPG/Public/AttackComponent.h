// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Common,
	Intensive1,
	Intensive2,
	Special1,
	Special2,
};

USTRUCT(BlueprintType)
struct FDamageRange
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDamageType damageType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float attackRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float attackLength = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 minDamage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 maxDamage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float pushForce = 0;
};


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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	TArray<class UInputAction*> inputarray;
	TArray<FHitResult>hitinfos;

	float currentTime = 0;

	UFUNCTION()
	void CommonAttack();
	UFUNCTION()
	void intensiveAttack();
	UFUNCTION()
	void SpecialAttack();

	UPROPERTY()
	class ASH_Player* player;
	UPROPERTY()
	class AHitObjectBase* HitObject;

	UPROPERTY(EditAnywhere, Category = AttackValue) // 맞았을때 e 딜레이 주는거
	float MinusDelayValue = 0.2f;
	UPROPERTY(EditAnywhere, Category = AttackValue) //Q 값 올라가는거
	float addPercent = 35.0f; 
	UPROPERTY(EditAnywhere, Category = AttackValue) //e딜레이 타임
	float intensiveDelay = 5;

	float specialCount = 0;
	int32 currDamage = 0;

public:	
	// Called every frame
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);

	FDamageRange CommonRange = {EDamageType::Common, 50.0, 80.0, 1, 2};
	FDamageRange IntensiveRange1 = {EDamageType::Intensive1, 80.0, 100.0, 3, 5};
	FDamageRange IntensiveRange2 = {EDamageType::Intensive2, 80.0, 100.0, 5, 7, 10.0};
	FDamageRange SpecialRange1 = {EDamageType::Special1, 150.0, 100.0, 1, 2};
	FDamageRange SpecialRange2 = {EDamageType::Special2, 150.0, 100.0, 15, 20, 20.0};

	UPROPERTY()
	class AEnemyBase* Target;
	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* playerMontage;
	UPROPERTY()
	TArray<class AActor*> ActorArr;
	UPROPERTY()
	TArray<class AEnemyBase*> TargetArr;

	void NextCombo();
	void DamageChange(FDamageRange damageRangeType);
	void TargetCheck(FDamageRange damageRange);
	void ResetAttack();

	FVector direction;
	FVector force;

	int32 attackCount = 0;
	bool isAttacking = false;
	bool isSpecialAttacking = false;
	bool goToNextCombo = false;
	bool coolTimeRunning = false;
	bool iscriticAttack = false;
};
