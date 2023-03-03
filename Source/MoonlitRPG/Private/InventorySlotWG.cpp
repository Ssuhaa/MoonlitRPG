// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include "InventoryWG.h"
#include "ItemDescriptionWG.h"


void UInventorySlotWG::NativeConstruct()
{
	Super::NativeConstruct();
	ItemButton->OnPressed.AddDynamic(this, &UInventorySlotWG::PopItemDescription);
}

void UInventorySlotWG::SetItemSlot()
{
	ItemImage->SetBrushFromTexture(iteminfo.iteminfomation.itemImage);
	if (iteminfo.itemAmont > 1)
	{
		ItemAmont->SetText(FText::FromString(FString::Printf(TEXT("%d"), iteminfo.itemAmont)));
	}
}

void UInventorySlotWG::PopItemDescription()
{
	invenWG->ItemSlotClicked(iteminfo);
}
