// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>

void UItemDescriptionWG::SetDescription(FInvenItem iteminfo)
{
	ItemImage->SetBrushFromTexture(iteminfo.iteminfomation.itemImage);
	ItemName->SetText(FText::FromString(iteminfo.iteminfomation.ItemName));
	Description->SetText(FText::FromString(iteminfo.iteminfomation.itemDescription));
}
