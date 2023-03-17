// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemBase.h"
#include "WeaponItemBase.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInvenItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIteminfo iteminfomation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponinfo weaponinfomaiton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 itemAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid InvenID = FGuid::NewGuid();

	bool operator==(const FInvenItem& Other) const //비교 연산자
	{
		return iteminfomation == Other.iteminfomation && 
			weaponinfomaiton == Other.weaponinfomaiton &&
			InvenID == Other.InvenID;
	}
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
	UPROPERTY()
	TArray<class UInputAction*> inputArray; //바인드

	TSubclassOf<class UInventoryWG> invenFactory;

	UPROPERTY()
	class ASH_Player* Player;

	void AddItemToinven(FIteminfo Getiteminfo, int32 Amount);
	void AddItemToinven(FIteminfo Getiteminfo, FWeaponinfo GetWeaponinfo);

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
	bool WeaponSwitch(FInvenItem ChangeItem);
	bool WeaponOff(FInvenItem ChangeItem);


	void CheckSameItemAfterAdd(FIteminfo iteminfo, int32 Amount);
	void CheckSameItemAfterAdd(FIteminfo iteminfo, FWeaponinfo weaponinfo);

	int32 MinusItemAmount(FInvenItem MinusInvenItem, int32 Amount);

	int32 FindItem(FIteminfo iteminfo);
	int32 FindItem(FInvenItem invenitem);
	int32 FindItem(FString ItemName);
	TArray<FInvenItem> FindAllItems(FIteminfo iteminfo);
	TArray<FInvenItem> FindAllItemsType(EItemType itemType);

	int32 CountItem();

};
