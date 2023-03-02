// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemBase.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInvenItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIteminfo iteminfomation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 itemAmont;
};

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
	TArray<class UInputAction*> inputArray; //���ε�


	TSubclassOf<class UInventoryWG> invenFactory;
	class UInventoryWG* inventory;


	class ASH_Player* Player;


	void InventoryOpen();

	UPROPERTY(EditAnywhere)
	int32 Money = 0;
	UPROPERTY(EditAnywhere)
	TArray<FInvenItem> invenItemArr;

public:	
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);

	void CheckSameItem(FIteminfo iteminfo);
	void AddInven(FIteminfo Getiteminfo);

};
