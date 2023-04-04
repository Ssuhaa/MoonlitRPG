// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include "ItemDescriptionWG.h"
#include "InventoryWG.h"




void UInventorySlotWG::ButtonBinding()
{
	ItemButton->OnPressed.AddUniqueDynamic(this, &UInventorySlotWG::SlotClicked);
}


void UInventorySlotWG::UpdateSlot(FInvenItem* invenitem)
{
	Super::UpdateSlot(invenitem);

	ItemAmount->SetText(FText::FromString(FString::Printf(TEXT("%d"), invenitem->itemAmount)));
	
}


void UInventorySlotWG::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(AddSlot);
}

void UInventorySlotWG::SlotClicked()
{
	InvenWG->ItemSlotClicked(&invenData);
}
