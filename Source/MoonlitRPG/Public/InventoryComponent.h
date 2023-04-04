// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemBase.h"
#include "WeaponItemBase.h"
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
	UPROPERTY()
	TArray<class UInputAction*> inputArray; //πŸ¿ŒµÂ

	TSubclassOf<class UInventoryWG> invenFactory;

	UPROPERTY()
	class ASH_Player* Player;

	UPROPERTY()
	class ADataManager* DataManager;

	//void CommonAddItemToinven(int32 ItemInfoIndex, int32 Amount);

	void InventoryOpen();


public:	
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);
	UPROPERTY(EditAnywhere)
	int32 Money = 0;
	UPROPERTY(EditAnywhere)
	TArray<FInvenItem>invenItemArr;
	UPROPERTY()
	class UInventoryWG* inventory;


	int32 CheckWeaponisEquip();
	bool WeaponSwitch(FInvenItem* ChangeItem);
	bool WeaponOff(FInvenItem* ChangeItem);


	void CommonCheckSameItemAfterAdd(int32 ItemInfoIndex, int32 Amount);
	void WeaponAddItemToinven(int32 ItemInfoIndex, int32 WeaponInfoIndex);


	int32 MinusItemAmount(int32 MinusInvenItem, int32 Amount);
	int32 GiveAllItem(int32 MinusItem);

	int32 FindItem(int32 iteminfoindex);
	int32 FindItem(FInvenItem* invenitem);
	TArray<FInvenItem*> FindAllItems(int32 itemindex);
	TArray<FInvenItem*> FindAllItemsType(EItemType itemType);

	int32 CountItem();
};
