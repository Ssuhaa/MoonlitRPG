// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include "InventoryWG.h"
#include "ItemDescriptionWG.h"



void UInventorySlotWG::ButtonBinding()
{
	ItemButton->OnPressed.AddDynamic(this, &UInventorySlotWG::PopItemDescription);
}

void UInventorySlotWG::SetItemSlot(FInvenItem iteminfo)
{
	selectiteminfo = iteminfo;
	ItemImage->SetBrushFromTexture(iteminfo.iteminfomation.itemImage);
	if (iteminfo.itemAmount > 1)
	{
		ItemAmount->SetText(FText::FromString(FString::Printf(TEXT("%d"), iteminfo.itemAmount)));
	}
	else if (iteminfo.itemAmount == 1)
	{
		ItemAmount->SetText(FText::FromString(FString(TEXT(" "))));
	}
	else
	{
		RemoveFromParent();
	}
}

void UInventorySlotWG::PopItemDescription()
{
	invenWG->ItemSlotClicked(Slotindex);
}
