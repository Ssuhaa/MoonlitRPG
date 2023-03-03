// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <UMG/Public/Blueprint/UserWidget.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "InventoryWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>

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
	inventory = CreateWidget<UInventoryWG>(GetWorld(), invenFactory);
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
		inventory->AddWidget();
	}
	else
	{
		inventory->RemoveWidget();
	}
}

void UInventoryComponent::CheckSameItemAfterAdd(FIteminfo iteminfo, int32 Amont) //아이템 추가 함수
{
	int32 value = FindItem(iteminfo);
	if (value > -1)
	{
		if (invenItemArr[value].iteminfomation.Stackalbe)
		{
			invenItemArr[value].itemAmont += Amont;
		}
	}
	else
	{
		AddItemToinven(iteminfo, Amont);
	}
}

void UInventoryComponent::AddItemToinven(FIteminfo Getiteminfo, int32 Amont) 
{
	FInvenItem currGetItem;
	currGetItem.iteminfomation = Getiteminfo;
	currGetItem.itemAmont = Amont;
	invenItemArr.Add(currGetItem);
}

bool UInventoryComponent::PlusMinusItemAmont(FIteminfo AdditemInfo, int32 Amont) // 기존에 있는 아이템 찾는
{
	int32 value = FindItem(AdditemInfo);
	if (value > -1)
	{
		if (Amont < 0)
		{
			if (invenItemArr[value].itemAmont < Amont)
			{
				return false;
			}
			else
			{
				invenItemArr[value].itemAmont += Amont;
				if (invenItemArr[value].itemAmont <= 0)
				{
					invenItemArr.RemoveAt(value);
				}
				return true;
			}
		}
		else
		{
			invenItemArr[value].itemAmont += Amont;
			if (invenItemArr[value].itemAmont <= 0)
			{
				invenItemArr.RemoveAt(value);
			}
			return true;
		}
	
	}
	else return false;
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
		value += invenItemArr[i].itemAmont;
	}
	return value;
}

