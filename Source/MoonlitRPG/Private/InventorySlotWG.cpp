// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include "ItemDescriptionWG.h"
#include "InventoryWG.h"

void UInventorySlotWG::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(AddSlot);
}

void UInventorySlotWG::ButtonBinding()
{
	ItemButton->OnPressed.AddUniqueDynamic(this, &UInventorySlotWG::SlotClicked);

}


void UInventorySlotWG::UpdateSlot(FInvenItem* invenitem)
{
	Super::UpdateSlot(invenitem);

	ItemAmount->SetText(FText::FromString(FString::Printf(TEXT("%d"), invenitem->itemAmount)));
	
}


void UInventorySlotWG::StopSelectAnim()
{
	StopAnimation(Select);
}

void UInventorySlotWG::PlaySelectAnim()
{
	PlayAnimation(Select, 0.0, 0);
}


void UInventorySlotWG::SlotClicked()
{
	InvenWG->StopSelectAnim();
	PlayAnimation(Click);
	PlaySelectAnim();
	InvenWG->ItemSlotClicked(&invenData);
}
