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
	void Run();
	void Walk();

	FVector dir;

	class ASH_Player* Player;
	class UCharacterMovementComponent* Movement;
	

	UPROPERTY(EditAnywhere, Category = WalkSpeed)
	float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, Category = WalkSpeed)
	float RunSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, Category = Stamina)
	float Stamina = 100.0f;
	UPROPERTY(EditAnywhere, Category = Stamina)
	float decreaseStamina = 1.0f;
	float maxStamina = 100.0f;
	float minStamina = 0.0f;

	class UPlayerMainWG* MainHud;

	bool isStaminaUse = false;
	float WalkToRunRatio = 0.0f; 

	void StaminaUpdate();

public:	
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);
	float currentStamina;
};
