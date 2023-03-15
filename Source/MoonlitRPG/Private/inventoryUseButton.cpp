// Fill out your copyright notice in the Description page of Project Settings.


#include "inventoryUseButton.h"
#include "InventoryWG.h"
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/TextBlock.h>

void UinventoryUseButton::SetText(FString Text)
{
	ButtonText->SetText(FText::FromString(Text));
}
