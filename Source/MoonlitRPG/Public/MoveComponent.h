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

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TArray <class UInputAction*> inputArray;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* IMC;

	void Horizontal(const struct FInputActionValue& value);
	void Vertical(const struct FInputActionValue& value);
	void Look(const struct FInputActionValue& value);
	void Jump();

	FVector dir;

	class ASH_Player* Player;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);


		
};
