// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponItemBase.h"
#include "AttackComponent.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Hand_Common,
	DG_Common,
	DG_Intensive1,
	DG_Intensive2,
	DG_Special1,
	DG_Special2,
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
	UPROPERTY()
	class AIH_Puzzle* HitPuzzle;
	UPROPERTY()
	TArray<class UStaticMeshComponent*> puzzleMesh;

	UPROPERTY(EditAnywhere, Category = AttackValue) // 맞았을때 e 딜레이 주는거
	float MinusDelayValue = 0.2f;
	UPROPERTY(EditAnywhere, Category = AttackValue) //Q 값 올라가는거
	float addPercent = 35.0f; 
	UPROPERTY(EditAnywhere, Category = AttackValue) //e딜레이 타임
	float intensiveDelay = 5;

	float specialCount = 0;
	int32 currDamage = 0;

	UPROPERTY()
	class ADataManager* DataManager;

public:	
	// Called every frame
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);

	//TArray<FDamageRange> damageRangeArr;

	FDamageRange CommonRange = {EDamageType::Hand_Common, 50.0f, 50.0f, 2, 3};
	// 단검 공격
	FDamageRange DG_CommonRange = {EDamageType::DG_Common, 50.0f, 80.0f, 5, 6};
	FDamageRange DG_IntensiveRange1 = {EDamageType::DG_Intensive1, 80.0f, 100.0f, 8, 10};
	FDamageRange DG_IntensiveRange2 = {EDamageType::DG_Intensive2, 80.0f, 100.0f, 10, 12, 5.0f};
	FDamageRange DG_SpecialRange1 = {EDamageType::DG_Special1, 150.0f, 100.0f, 5, 6};
	FDamageRange DG_SpecialRange2 = {EDamageType::DG_Special2, 150.0f, 100.0f, 15, 20, 10.0f};
	// 두손검 공격
	FDamageRange GS_CommonRange = {EDamageType::GS_Common, 80.0f, 80.0f, 8, 10, 1.0f};
	FDamageRange GS_IntensiveRange1 = {EDamageType::GS_Intensive1, 100.0f, 100.0f, 10, 12};
	FDamageRange GS_IntensiveRange2 = { EDamageType::GS_Intensive2, 100.0f, 100.0f, 13, 15, 5.0f };
	FDamageRange GS_SpecialRange1 = {EDamageType::GS_Special1, 150.0f, 100.0f, 20, 25, 15.0f};
	FDamageRange GS_SpecialRange2 = {EDamageType::GS_Special2, 100.0f, 100.0f, 13, 15};

	UPROPERTY()
	class AEnemyBase* Target;
	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* damagedMontage;
	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* handMontage;
	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* daggerMontage;
	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* swordMontage;
	UPROPERTY()
	TArray<class AActor*> ActorArr;
	UPROPERTY()
	TArray<class AEnemyBase*> TargetArr;

	EWeaponType currWeapon = EWeaponType::None;

	void NextCombo();
	void DamageChange(FDamageRange damageRangeType);
	void TargetCheck(FDamageRange damageRange);
	void ResetAttack();
	void EnemyAttack(FDamageRange damageRange);
	void PlayAttackMontage(FString montName);
	void WeaponChange(int32 WeaponinfoIndex);
	void ImpactEffect(FVector impactLoc);
	
	FVector direction;
	FVector force;

	int32 attackCount = 0;
	float skillPercent = 1.0f;
	bool isAttacking = false;
	bool isSpecialAttacking = false;
	bool goToNextCombo = false;
	bool coolTimeRunning = false;
	bool iscriticAttack = false;
};
