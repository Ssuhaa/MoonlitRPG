// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>
#include "InventorySlotWG.h"
#include <UMG/Public/Components/ScrollBox.h>
#include <UMG/Public/Components/Overlay.h>


void UInventoryWG::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Close->OnClicked.AddDynamic(this, &UInventoryWG::RemoveWidget);
}

void UInventoryWG::Setinventory(TArray<FInvenItem> invenArr)
{
	if(!invenArr.IsValidIndex(0)) return;
	ItemScrollBox->ClearChildren();
	for (int32 i=0; i<invenArr.Num();i++)
	{
		UInventorySlotWG* currslot = CreateWidget<UInventorySlotWG> (GetWorld(), SlotFactory);
		currslot->invenWG = this;
		currslot->SetItemSlot(invenArr[i]);
		ItemScrollBox->AddChild(currslot);
	}
}

void UInventoryWG::RemoveWidget()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UInventoryWG::AddWidget(TArray<FInvenItem> invenArr)
{
	Setinventory(invenArr);
	AddToViewport();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
}


void UInventoryWG::ItemSlotClicked()
{
	Overlay_ItemInfo->ClearChildren();
}