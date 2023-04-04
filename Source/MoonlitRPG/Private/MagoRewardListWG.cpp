// Fill out your copyright notice in the Description page of Project Settings.


#include "MagoRewardListWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>

void UMagoRewardListWG::SetMagoRewardList(class UTexture2D* image, FText itemName, int32 Amount)
{
	image_Item->SetBrushFromTexture(image);
	TB_itemName->SetText(itemName);
	TB_Amount->SetText(FText::AsNumber(Amount));
}
