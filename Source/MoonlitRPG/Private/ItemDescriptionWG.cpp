// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>

void UItemDescriptionWG::SetDescription(FInvenItem iteminfo)
{	
	if(ItemImage != nullptr)
	{
		ItemImage->SetBrushFromTexture(iteminfo.iteminfomation.itemImage);
	}
	if (ItemName != nullptr)
	{
		ItemName->SetText(FText::FromString(iteminfo.iteminfomation.ItemName));
	}
	if (Description != nullptr)
	{
		Description->SetText(FText::FromString(iteminfo.iteminfomation.itemDescription));
	}

}
