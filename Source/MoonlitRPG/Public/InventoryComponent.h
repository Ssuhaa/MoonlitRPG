// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOONLITRPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	TSubclassOf<class UInventoryWG> invenFactory;
	class UInventoryWG* inventory;

	TArray<class UInputAction*> inputArray;

	class ASH_Player* Player;

public:	
	void InventoryOpen();

	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);
};
