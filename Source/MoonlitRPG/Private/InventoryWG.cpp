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
#include <UMG/Public/Components/TextBlock.h>



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
	Button_Right->OnClicked.AddDynamic(this, &UInventoryWG::RightMenu);
	Button_Left->OnClicked.AddDynamic(this, &UInventoryWG::LeftMenu);
}


void UInventoryWG::RemoveWidget()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UInventoryWG::AddWidget(TArray<FInvenItem> invenArr, int32 money)
{
	invenArray = invenArr;
	CurrMoney = money; 
	Setinventory();
	Money_Text->SetText(FText::FromString(FString::Printf(TEXT("%d"),CurrMoney)));
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

void UInventoryWG::LeftMenu()
{
	int32 index = int32(currinventype);
	index--;
	index = FMath::Clamp(index, 0, int32(EItemType::Count) - 1);
	currinventype = EItemType(index);
	Setinventory();
}

void UInventoryWG::RightMenu()
{
	int32 index = int32(currinventype);
	index++;
	index = FMath::Clamp(index, 0, int32(EItemType::Count) - 1);
	currinventype = EItemType(index);
	Setinventory();
}


void UInventoryWG::Setinventory()
{
	if (!invenArray.IsValidIndex(0)) return;
	WrapBox->ClearChildren();
	Overlay_ItemInfo->ClearChildren();

	FText currslotName;

	switch (currinventype)
	{	
	case EItemType::Consum:
		currslotName = FText::FromString(TEXT("소비창"));
		break;
	case EItemType::Outfit:
		currslotName = FText::FromString(TEXT("장비창"));
		break;
	case EItemType::Quest :
		currslotName = FText::FromString(TEXT("퀘스트창"));
		break;
	case EItemType::Food :
		currslotName = FText::FromString(TEXT("음식창"));
		break;
	}

	CurrSlot_Text->SetText(currslotName);

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
