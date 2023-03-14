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
	GS_Common,
	GS_Intensive1,
	GS_Intensive2,
	GS_Special1,
	GS_Special2
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

	//TArray<FDamageRange> damageRangeArr;

	// 단검 공격
	FDamageRange CommonRange = {EDamageType::Common, 50.0f, 80.0f, 1, 2};
	FDamageRange IntensiveRange1 = {EDamageType::Intensive1, 80.0f, 100.0f, 2, 3};
	FDamageRange IntensiveRange2 = {EDamageType::Intensive2, 80.0f, 100.0f, 3, 5, 10.0f};
	FDamageRange SpecialRange1 = {EDamageType::Special1, 150.0f, 100.0f, 1, 2};
	FDamageRange SpecialRange2 = {EDamageType::Special2, 150.0f, 100.0f, 7, 10, 20.0f};
	// 두손검 공격
	FDamageRange GS_CommonRange = {EDamageType::GS_Common, 80.0f, 80.0f, 2, 3, 1.0f};
	FDamageRange GS_IntensiveRange1 = {EDamageType::GS_Intensive1, 100.0f, 100.0f, 3, 5};
	FDamageRange GS_IntensiveRange2 = { EDamageType::GS_Intensive2, 100.0f, 100.0f, 3, 5, 5.0f };
	FDamageRange GS_SpecialRange1 = {EDamageType::GS_Special1, 150.0f, 100.0f, 10, 12, 5.0f};
	FDamageRange GS_SpecialRange2 = {EDamageType::GS_Special2, 100.0f, 100.0f, 5, 7};

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
	void EnemyAttack(FDamageRange damageRange);
	void PlayAttackMontage(class UAnimMontage* montage, FString montName);

	FVector direction;
	FVector force;

	int32 attackCount = 0;
	bool isAttacking = false;
	bool isSpecialAttacking = false;
	bool goToNextCombo = false;
	bool coolTimeRunning = false;
	bool iscriticAttack = false;

	UPROPERTY(EditAnywhere)
	bool bEquipGS = false;		// 임시 변수(두손검 몽타주 테스트)
};
