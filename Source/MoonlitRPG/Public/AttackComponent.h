// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Common,
	Intensive,
	Special,
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
	float damage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float pushForce  = 0;

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

	void CommonAttack();
	void intensiveAttack();
	void SpecialAttack();

	UPROPERTY()
	class ASH_Player* player;
	UPROPERTY()
	class AHitObjectBase* HitObject;

	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* attackMontage;


	UPROPERTY(EditAnywhere, Category = AttackValue) // 맞았을때 e 딜레이 주는거
	float MinusDelayValue = 0.2f;
	UPROPERTY(EditAnywhere, Category = AttackValue) //Q 값 올라가는거
	float addPercent = 35.0f; 
	UPROPERTY(EditAnywhere, Category = AttackValue) //e딜레이 타임
	float intensiveDelay = 5;

	float specialCount = 0;


public:	
	// Called every frame
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);

	FDamageRange commonRange = { EDamageType::Common, 50, 80, 1, 0 };
	FDamageRange IntensiveRange = { EDamageType::Intensive, 80, 100, 1, 15 };
	FDamageRange SpecialRange = { EDamageType::Special, 150 , 100, 5, 30 };

	UPROPERTY()
	class AEnemyBase* Target;

	void NextCombo();
	void TargetCheck(FDamageRange damageRange);

	FVector direction;
	FVector force;


	int32 attackCount = 0;
	bool isAttacking = false;
	bool goToNextCombo = false;
	bool coolTimeRunning = false;

};
