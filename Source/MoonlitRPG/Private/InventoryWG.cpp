// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/ScrollBox.h>
#include <UMG/Public/Components/Overlay.h>
#include <UMG/Public/Components/WrapBox.h>
#include <UMG/Public/Components/TextBlock.h>
#include "InventoryComponent.h"
#include "InventorySlotWG.h"
#include "ItemDescriptionWG.h"
#include "inventoryUseButton.h"



UInventoryWG::UInventoryWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UInventorySlotWG> tempslot(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_InvenSlot.BP_InvenSlot_C'"));
	if (tempslot.Succeeded())
	{
		SlotFactory = tempslot.Class;
	}
	ConstructorHelpers::FClassFinder <UinventoryUseButton> tempButton(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_InvenUseButton.BP_InvenUseButton_C'"));
	if (tempButton.Succeeded())
	{
		ButtonFactory = tempButton.Class;
	}
	ConstructorHelpers::FClassFinder <UItemDescriptionWG> tempDesrip(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_ItemDescription.BP_ItemDescription_C'"));
	if (tempDesrip.Succeeded())
	{
		DescriptionFactory = tempDesrip.Class;
	}
}

void UInventoryWG::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Close->OnPressed.AddDynamic(this, &UInventoryWG::RemoveWidget);
	Button_Consum->OnPressed.AddDynamic(this, &UInventoryWG::ClickedConsum); 
	Button_Outfit->OnPressed.AddDynamic(this, &UInventoryWG::ClickedOutfit);
	Button_Food->OnPressed.AddDynamic(this, &UInventoryWG::ClickedFood);
	Button_Quest->OnPressed.AddDynamic(this, &UInventoryWG::ClickedQuest);
	Button_Right->OnPressed.AddDynamic(this, &UInventoryWG::RightMenu);
	Button_Left->OnPressed.AddDynamic(this, &UInventoryWG::LeftMenu);

	ButtonWG = CreateWidget<UinventoryUseButton>(GetWorld(), ButtonFactory);
	ButtonWG->InvenWG = this;
	ButtonWG->Button_Use->OnPressed.AddDynamic(this, &UInventoryWG::ClickedUseButton);
	Description = CreateWidget<UItemDescriptionWG>(GetWorld(), DescriptionFactory);
}


void UInventoryWG::RemoveWidget()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UInventoryWG::AddWidget()
{
	AddToViewport();
	Setinventory();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void UInventoryWG::ItemSlotClicked(FInvenItem currSelectItem)
{
	SelectedSlot = currSelectItem;
	Overlay_ItemInfo->ClearChildren();
	Overlay_Use->ClearChildren();

	Description->SetDescription(SelectedSlot); //키고난 후 
	Overlay_ItemInfo->AddChildToOverlay(Description);
	if (SelectedSlot.iteminfomation.itemType == EItemType::Food)
	{
		Overlay_Use->AddChildToOverlay(ButtonWG);
	}
	//장비 착용 버튼 추가
}

void UInventoryWG::ClickedUseButton()
{
	InvenComp->PlusMinusItemAmont(SelectedSlot.iteminfomation, -1);
	//HP 회복 호출
	Setinventory();
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
	if(!InvenComp->invenItemArr.IsValidIndex(0)) return;

	WrapBox->ClearChildren();
	Overlay_Use->ClearChildren();
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

	for (int32 i = 0; i < InvenComp->invenItemArr.Num(); i++)
	{
	
		if (InvenComp->invenItemArr[i].iteminfomation.itemType == currinventype)
		{
			UInventorySlotWG* currslot = CreateWidget<UInventorySlotWG>(GetWorld(), SlotFactory);
			currslot->invenWG = this;
			currslot->iteminfo = InvenComp->invenItemArr[i];
			currslot->SetItemSlot();
			WrapBox->AddChildToWrapBox(currslot);
		}
	}

	if (Money_Text != nullptr)
	{
		Money_Text->SetText(FText::FromString(FString::FromInt(InvenComp->Money)));
	}

	if (Text_Count != nullptr)
	{
		Text_Count->SetText(FText::FromString(FString::FromInt(InvenComp->CountItem())));
	}
}
