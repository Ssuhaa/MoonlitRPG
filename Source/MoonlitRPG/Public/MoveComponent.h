// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOONLITRPG_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TArray <class UInputAction*> inputArray;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* IMC;

	void Horizontal(const struct FInputActionValue& value);
	void Vertical(const struct FInputActionValue& value);
	void Look(const struct FInputActionValue& value);
	void Jump();
	void Dash();

	FVector dir;

	class ASH_Player* Player;
	class UCharacterMovementComponent* Movement;
	

	UPROPERTY(EditAnywhere, Category = WalkSpeed)
	float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, Category = WalkSpeed)
	float RunSpeed = 600.0f;
	UPROPERTY(EditAnywhere, Category = WalkSpeed)
	float DashSpeed = 1200.0f;
	UPROPERTY(VisibleAnywhere, Category = WalkSpeed)
	float CurrSpeed;

	void DashToWalk();
	void SetWalkSpped(float MinSpeed, float MaxSpeed, float MinusSpeed);

	UPROPERTY(VisibleAnywhere, Category = Stamina)
	float minStamina = 0.0f;
	UPROPERTY(EditAnywhere, Category = Stamina)
	float maxStamina = 100.0f;
	UPROPERTY(VisibleAnywhere, Category = Stamina)
	float Stamina = 100.0f;
	UPROPERTY(EditAnywhere, Category = Stamina)
	float decreaseStamina = 0.5f;

	bool isStaminaUse = false;
	void StaminaUpdate();

	UPROPERTY()
	class UPlayerMainWG* MainHud;

	UPROPERTY()
	class UAnimMontage* MoveMontage;


	bool PlayerMoveCheck();
	bool isPlayerMove = false;
	bool isAir= false;
	float dirH;
	float dirV;

public:	
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);
	float currentStamina;
};
