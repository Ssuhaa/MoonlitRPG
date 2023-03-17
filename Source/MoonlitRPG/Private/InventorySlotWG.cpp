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


void UInventorySlotWG::UpdateSlot(FInvenItem invenData)
{
	Super::UpdateSlot(invenData);
	if (invenInfo.itemAmount >= 1)
	{
		ItemAmount->SetText(FText::FromString(FString::Printf(TEXT("%d"), invenInfo.itemAmount)));
	}
}


void UInventorySlotWG::SlotClicked()
{
	invenWG->ItemSlotClicked(Slotindex);
}
