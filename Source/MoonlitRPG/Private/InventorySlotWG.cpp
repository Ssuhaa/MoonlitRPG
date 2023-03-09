// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include "InventoryWG.h"
#include "ItemDescriptionWG.h"
#include <Engine/Texture2D.h>


UInventorySlotWG::UInventorySlotWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Slot_Common.T_Slot_Common'"));
	if (tempT.Succeeded())
	{
		BGarray.Add(tempT.Object); //0번 Common
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT1(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Slot_Rare_.T_Slot_Rare_'"));
	if (tempT1.Succeeded())
	{
		BGarray.Add(tempT1.Object);	//1번 Rare
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT2(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Slot_Uinque.T_Slot_Uinque'"));
	if (tempT2.Succeeded())
	{
		BGarray.Add(tempT2.Object); //2번 Uinque
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT3(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Slot_Legendary.T_Slot_Legendary'"));
	if (tempT3.Succeeded())
	{
		BGarray.Add(tempT3.Object);	//3번 Legendary
	}
}

void UInventorySlotWG::ButtonBinding()
{
	ItemButton->OnPressed.AddDynamic(this, &UInventorySlotWG::PopItemDescription);
}

void UInventorySlotWG::SetItemSlot(FInvenItem* iteminfo)
{
	selectiteminfo = iteminfo;
	UpdateSlot();
}


void UInventorySlotWG::UpdateSlot()
{
	ItemImage->SetBrushFromTexture(selectiteminfo->iteminfomation.itemImage);
	int32 BGindex = int32(selectiteminfo->iteminfomation.itemgrade);
	SlotBG->SetBrushFromTexture(BGarray[BGindex], true);
	if (selectiteminfo->itemAmount >= 1)
	{
		ItemAmount->SetText(FText::FromString(FString::Printf(TEXT("%d"), selectiteminfo->itemAmount)));
	}
}

void UInventorySlotWG::PopItemDescription()
{
	invenWG->ItemSlotClicked(Slotindex);
}
