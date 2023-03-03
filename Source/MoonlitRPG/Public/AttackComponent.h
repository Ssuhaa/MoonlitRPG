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

	void CommonAttack();
	void intensiveAttack();
	void SpecialAttack();

	class ASH_Player* player;
	class AIH_Enemy* Target;

public:
	UPROPERTY(EditAnywhere, Category = Range)
	float attackRange = 80.0f;
	
	UPROPERTY(EditAnywhere, Category = Range)
	float attackRadius = 50.0;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* attackMontage;

	bool isAttacking = false;
	bool saveAttack = false;
	int32 attackCount = 0;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);
	
	void ComboAttackSave();
	void ComboReset();
};
