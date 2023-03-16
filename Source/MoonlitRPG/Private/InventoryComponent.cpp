// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <UMG/Public/Blueprint/UserWidget.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "InventoryWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "AttackComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_Tab.Key_Tab'"));
	if (tempAction.Succeeded())
	{
		inputArray.Add(tempAction.Object); //0번 탭
	}
	ConstructorHelpers::FClassFinder<UInventoryWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_WG_Inventory.BP_WG_Inventory_C'"));
	if (tempWG.Succeeded())
	{
		invenFactory = tempWG.Class; 
	}
	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASH_Player>(GetOwner());
	inventory = CreateWidget<UInventoryWG>(GetWorld(),invenFactory);
	inventory->InvenComp = this;
}



// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent)
{
	if (inputArray.IsValidIndex(0))
	{
		EnhancedInputComponent->BindAction(inputArray[0], ETriggerEvent::Triggered, this, &UInventoryComponent::InventoryOpen); // Tab
	}
}


void UInventoryComponent::InventoryOpen()
{
	if (!inventory->IsInViewport())
	{
		inventory->AddToViewport();
	}
	else
	{
		inventory->RemoveInventory();
	}
}

void UInventoryComponent::CheckSameItemAfterAdd(FIteminfo iteminfo, int32 Amount) // 일반 아이템 추가 함수
{
	int32 value = FindItem(iteminfo);
	if (value > -1)
	{
		if (invenItemArr[value].iteminfomation.Stackalbe)
		{
			invenItemArr[value].itemAmount += Amount;
		}
	}
	else
	{
		AddItemToinven(iteminfo, Amount);
	}
}

void UInventoryComponent::CheckSameItemAfterAdd(FIteminfo iteminfo, FWeaponinfo weaponinfo)
{
	AddItemToinven(iteminfo, weaponinfo);
}

int32 UInventoryComponent::CheckWeaponisEquip()
{
	for (int32 i = 0; i < invenItemArr.Num() ; i++)
	{
		if (invenItemArr[i].weaponinfomaiton.isEquip)
		{
			return i;
		}
	}

	return -1;
}


EWeaponType UInventoryComponent::WhatKindOfEquipWeapon()
{
	int32 index = CheckWeaponisEquip();
	if (index > -1)
	{
		return invenItemArr[index].weaponinfomaiton.WeaponType;
	}
	else
	{
		return EWeaponType::None;
	}
}


void UInventoryComponent::WeaponSwitch(FInvenItem* ChangeItem)
{	
	int32 currEquip = CheckWeaponisEquip();
	if (currEquip > -1)
	{
		invenItemArr[currEquip].weaponinfomaiton.isEquip = false;
	}
	
	ChangeItem->weaponinfomaiton.isEquip = true;
	Player->AttackComp->WeaponChange(ChangeItem->weaponinfomaiton.WeaponType);
}

void UInventoryComponent::AddItemToinven(FIteminfo Getiteminfo, int32 Amount)
{
	FInvenItem currGetItem;
	currGetItem.iteminfomation = Getiteminfo;
	currGetItem.itemAmount = Amount;
	invenItemArr.Add(currGetItem);

}

void UInventoryComponent::AddItemToinven(FIteminfo Getiteminfo, FWeaponinfo GetWeaponinfo)
{
	FInvenItem currGetItem;
	currGetItem.iteminfomation = Getiteminfo;
	currGetItem.weaponinfomaiton = GetWeaponinfo;
	currGetItem.itemAmount = 1;
	invenItemArr.Add(currGetItem);
}

int32 UInventoryComponent::PlusMinusItemAmount(FIteminfo AdditemInfo, int32 Amount) 
{
	int32 value = FindItem(AdditemInfo);
	if (value > -1)
	{
		invenItemArr[value].itemAmount += Amount;
		int32 result = invenItemArr[value].itemAmount;
		if (invenItemArr[value].itemAmount < 1)
		{
			//invenItemArr[value].weaponinfomaiton.SendLevelUpClear.Clear();
			invenItemArr.RemoveAt(value);
		}
		return result;
	}
	return -1;
}

int32 UInventoryComponent::FindItem(FIteminfo iteminfo)
{
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		if (invenItemArr[i].iteminfomation.ItemName == iteminfo.ItemName)
		{
			return i;
		}
	}
	return -1;
}

int32 UInventoryComponent::CountItem()
{
	int32 value = 0;
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		value += invenItemArr[i].itemAmount;
	}
	return value;
}

void UInventoryComponent::Test(TArray<FInvenItem*> invendata)
{
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{	
		for (int32 j = 0; j < invendata.Num(); j++)
		{
			if (&invenItemArr[i] == invendata[j])
			{
				invenItemArr[i].itemAmount += 1;
				if (invenItemArr[i].itemAmount < 1)
				{
					invenItemArr.RemoveAt(i);
				}
				break;
			}
		}
	}

}

