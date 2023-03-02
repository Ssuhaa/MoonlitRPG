// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include "InventoryWG.h"

void UInventorySlotWG::NativeConstruct()
{
	Super::NativeConstruct();
	ItemButton->OnClicked.AddDynamic(invenWG, &UInventoryWG::ItemSlotClicked);
	ItemButton->OnClicked.AddDynamic(this, &UInventorySlotWG::PopItemDiscription);
}

void UInventorySlotWG::SetItemSlot(FInvenItem iteminfo)
{

	ItemImage->SetBrushFromTexture(iteminfo.iteminfomation.itemImage);
	ItemAmont->SetText(FText::FromString(FString::Printf(TEXT("%d"), iteminfo.itemAmont)));

}

void UInventorySlotWG::PopItemDiscription()
{
	
}
