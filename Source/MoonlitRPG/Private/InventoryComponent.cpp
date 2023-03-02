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
		inputArray.Add(tempAction.Object); //0╧Ь ег
	}
	ConstructorHelpers::FClassFinder<UUserWidget> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_WG_Inventory.BP_WG_Inventory_C'"));
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
		inventory->AddWidget(invenItemArr, Money);
	}
	else
	{
		inventory->RemoveWidget();
	}
}

void UInventoryComponent::CheckSameItem(FIteminfo iteminfo)
{
	bool bisSame = false;
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		if (invenItemArr[i].iteminfomation.ItemName == iteminfo.ItemName)
		{
			if (invenItemArr[i].iteminfomation.Stackalbe)
			{
				invenItemArr[i].itemAmont += 1;
				bisSame = true;
				break;
			}
		}
	}

	if (bisSame == false)
	{
		AddInven(iteminfo);
	}
}

void UInventoryComponent::AddInven(FIteminfo Getiteminfo)
{
	FInvenItem currGetItem;
	currGetItem.iteminfomation = Getiteminfo;
	currGetItem.itemAmont = 1;
	invenItemArr.Add(currGetItem);
}
