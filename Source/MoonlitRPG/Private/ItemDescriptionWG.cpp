// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include "InventorySlotWG.h"

UItemDescriptionWG::UItemDescriptionWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	ConstructorHelpers::FObjectFinder <UTexture2D> tempT(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/DescriptionBG_Common.DescriptionBG_Common'"));
	if (tempT.Succeeded())
	{
		BGarray.Add(tempT.Object); //0번 Common
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT1(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/DescriptionBG_RA.DescriptionBG_RA'"));
	if (tempT1.Succeeded())
	{
		BGarray.Add(tempT1.Object);	//1번 Rare
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT2(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/DescriptionUinque_Rare.DescriptionUinque_Rare'"));
	if (tempT2.Succeeded())
	{
		BGarray.Add(tempT2.Object); //2번 Uinque
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT3(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/DescriptionBG_Legendary.DescriptionBG_Legendary'"));
	if (tempT3.Succeeded())
	{
		BGarray.Add(tempT3.Object);	//3번 Legendary
	}
}

void UItemDescriptionWG::SetDescription(UInventorySlotWG* SelectedSlot)
{	
	FInvenItem* iteminfo = SelectedSlot->selectiteminfo;
	ItemImage->SetBrushFromTexture(iteminfo->iteminfomation.itemImage);
	ItemName->SetText(FText::FromString(iteminfo->iteminfomation.ItemName));
	Description->SetText(FText::FromString(iteminfo->iteminfomation.itemDescription));

	int32 BGindex = int32(iteminfo->iteminfomation.itemgrade);
	BG->SetBrushFromTexture(BGarray[BGindex], true);
}
