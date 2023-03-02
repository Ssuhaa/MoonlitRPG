// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>
#include "InventorySlotWG.h"
#include <UMG/Public/Components/ScrollBox.h>
#include <UMG/Public/Components/Overlay.h>
#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/WrapBox.h>



UInventoryWG::UInventoryWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UInventorySlotWG> tempslot(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_InvenSlot.BP_InvenSlot_C'"));
	if (tempslot.Succeeded())
	{
		SlotFactory = tempslot.Class;
	}
}

void UInventoryWG::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Close->OnClicked.AddDynamic(this, &UInventoryWG::RemoveWidget);
	Button_Consum->OnClicked.AddDynamic(this, &UInventoryWG::ClickedConsum);
	Button_Outfit->OnClicked.AddDynamic(this, &UInventoryWG::ClickedOutfit);
	Button_Food->OnClicked.AddDynamic(this, &UInventoryWG::ClickedFood);
	Button_Quest->OnClicked.AddDynamic(this, &UInventoryWG::ClickedQuest);
}


void UInventoryWG::RemoveWidget()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UInventoryWG::AddWidget(TArray<FInvenItem> invenArr)
{
	invenArray = invenArr;
	Setinventory();
	AddToViewport();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void UInventoryWG::ItemSlotClicked(class UItemDescriptionWG* DescriptionWG)
{
	Overlay_ItemInfo->ClearChildren();
	Overlay_ItemInfo->AddChildToOverlay(DescriptionWG);
}

void UInventoryWG::ClickedConsum()
{
	currinventype = EItemType::Consum;
	Setinventory();
}

void UInventoryWG::ClickedOutfit()
{
	currinventype = EItemType::Outfit;
	Setinventory();
}


void UInventoryWG::ClickedFood()
{
	currinventype = EItemType::Food;
	Setinventory();
}

void UInventoryWG::ClickedQuest()
{
	currinventype = EItemType::Quest;
	Setinventory();
}

void UInventoryWG::Setinventory()
{
	if (!invenArray.IsValidIndex(0)) return;
	WrapBox->ClearChildren();
	Overlay_ItemInfo->ClearChildren();

	for (int32 i = 0; i < invenArray.Num(); i++)
	{
		if (invenArray[i].iteminfomation.itemType == currinventype)
		{
			UInventorySlotWG* currslot = CreateWidget<UInventorySlotWG>(GetWorld(), SlotFactory);
			currslot->invenWG = this;
			currslot->iteminfo = invenArray[i];
			currslot->SetItemSlot();
			WrapBox->AddChildToWrapBox(currslot);
		}
	}

}
