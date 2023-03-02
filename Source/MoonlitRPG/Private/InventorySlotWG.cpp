// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include "InventoryWG.h"
#include "ItemDescriptionWG.h"

UInventorySlotWG::UInventorySlotWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UItemDescriptionWG> tempDesrip(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_ItemDescription.BP_ItemDescription_C'"));
	if (tempDesrip.Succeeded())
	{
		DescriptionFactory = tempDesrip.Class;
	}
}

void UInventorySlotWG::NativeConstruct()
{
	Super::NativeConstruct();
	Description = CreateWidget<UItemDescriptionWG> (GetWorld(), DescriptionFactory);
	ItemButton->OnClicked.AddDynamic(this, &UInventorySlotWG::PopItemDescription);
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
	Description->iteminfo = iteminfo;
	Description->SetDescription();
	invenWG->ItemSlotClicked(Description);
}
