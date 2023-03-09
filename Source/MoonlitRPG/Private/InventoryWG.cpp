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
#include "FoodPopup.h"
#include <UMG/Public/Components/ScaleBox.h>





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
		ButtonFactory.Add(tempButton.Class) ; //0번 사용하기
	}
	ConstructorHelpers::FClassFinder <UinventoryUseButton> tempButton1(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_InvenOutfitUseButton.BP_InvenOutfitUseButton_C'"));
	if (tempButton1.Succeeded())
	{
		ButtonFactory.Add(tempButton1.Class); //1번 착용하기
	}
	ConstructorHelpers::FClassFinder <UItemDescriptionWG> tempDesrip(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_ItemDescription.BP_ItemDescription_C'"));
	if (tempDesrip.Succeeded())
	{
		DescriptionFactory = tempDesrip.Class;
	}
	ConstructorHelpers::FClassFinder <UFoodPopup> tempfoodPop(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_FoodPopup.WG_FoodPopup_C'"));
	if (tempfoodPop.Succeeded())
	{
		FoodPopFactory = tempfoodPop.Class;
	}


	ButtonWG = CreateWidget<UinventoryUseButton>(GetWorld(), ButtonFactory[0]);
	OutfitButtonWG = CreateWidget<UinventoryUseButton>(GetWorld(), ButtonFactory[1]);
	Description = CreateWidget<UItemDescriptionWG>(GetWorld(), DescriptionFactory);
	FoodPopup = CreateWidget<UFoodPopup>(GetWorld(), FoodPopFactory);


	for (int32 i = 0; i < 100; i++)
	{
		UInventorySlotWG* currslot = CreateWidget<UInventorySlotWG>(GetWorld(), SlotFactory);
		Slots.Add(currslot);
	}

}

void UInventoryWG::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	ButtonBinding();
	ButtonWG->InvenWG = this;
	OutfitButtonWG->InvenWG = this;
	FoodPopup->invenWG = this;
}

void UInventoryWG::ButtonBinding()
{
	if (!Button_Close->OnPressed.IsBound())
	{
		Button_Close->OnPressed.AddDynamic(this, &UInventoryWG::RemoveWidget);
		Button_Consum->OnPressed.AddDynamic(this, &UInventoryWG::ClickedConsum);
		Button_Outfit->OnPressed.AddDynamic(this, &UInventoryWG::ClickedOutfit);
		Button_Food->OnPressed.AddDynamic(this, &UInventoryWG::ClickedFood);
		Button_Quest->OnPressed.AddDynamic(this, &UInventoryWG::ClickedQuest);
		Button_Right->OnPressed.AddDynamic(this, &UInventoryWG::RightMenu);
		Button_Left->OnPressed.AddDynamic(this, &UInventoryWG::LeftMenu);

		ButtonWG->Button_Use->OnPressed.AddDynamic(this, &UInventoryWG::ClickedUseButton);
		OutfitButtonWG->Button_Use->OnPressed.AddDynamic(this, &UInventoryWG::ClickedOutfitButton);

		FoodPopup->ButtonBinding();

		for (int32 i = 0; i < Slots.Num(); i++)
		{
			Slots[i]->ButtonBinding();
		}

	}
}


void UInventoryWG::RemoveWidget()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	if (FoodPopup->IsInViewport())
	{
		FoodPopup->RemoveFromParent();
	}
	RemoveFromParent();
}


void UInventoryWG::AddWidget()
{
	AddToViewport();
	Setinventory();
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UInventoryWG::ItemSlotClicked(int32 slotindex)
{
	Description->SetDescription(Slots[slotindex]);
	itemDescription->AddChild(Description);
	if (Slots[slotindex]->selectiteminfo->iteminfomation.itemType == EItemType::Food)
	{
		FoodPopup->SetFoodPop(Slots[slotindex]);
		Overlay_Use->ClearChildren();
		Overlay_Use->AddChildToOverlay(ButtonWG);

	}
	if (Slots[slotindex]->selectiteminfo->iteminfomation.itemType == EItemType::Outfit)
	{
		Overlay_Use->ClearChildren();
		Overlay_Use->AddChildToOverlay(OutfitButtonWG);
	}
}

void UInventoryWG::ClickedUseButton()
{
	FoodPopup->AddToViewport();
}


void UInventoryWG::ClickedOutfitButton()
{

}

void UInventoryWG::ClickedConsum()
{
	ChangeInven(EItemType::Consum);

}

void UInventoryWG::ClickedOutfit()
{

	ChangeInven(EItemType::Outfit);
}


void UInventoryWG::ClickedFood()
{
	ChangeInven(EItemType::Food);
}

void UInventoryWG::ClickedQuest()
{
	ChangeInven(EItemType::Quest);
}

void UInventoryWG::LeftMenu()
{
	int32 index = int32(currinventype);
	index--;
	if (index < 0)
	{
		index = int32(EItemType::Count) - 1;
	}
	ChangeInven(EItemType(index));
}

void UInventoryWG::RightMenu()
{
	int32 index = int32(currinventype);
	index++;
	if (index > int32(EItemType::Count) - 1)
	{
		index = 0;
	}
	ChangeInven(EItemType(index));
}

void UInventoryWG::ChangeInven(EItemType ChangeInvenType)
{
	if (currinventype == ChangeInvenType) return;

	currinventype = ChangeInvenType;
	ClearOverlay();
	WrapBox->ClearChildren();
	if (InvenComp->invenItemArr.IsValidIndex(0))
	{
		for (int32 i = 0; i < InvenComp->invenItemArr.Num(); i++)
		{
			if (InvenComp->invenItemArr[i].iteminfomation.itemType == currinventype)
			{
				if (!Slots.IsValidIndex(i))
				{
					UInventorySlotWG* currslot = CreateWidget<UInventorySlotWG>(GetWorld(), SlotFactory);
					Slots.Add(currslot);
					Slots[i]->ButtonBinding();
				}
				Slots[i]->invenWG = this;
				Slots[i]->Slotindex = i;
				Slots[i]->SetItemSlot(&InvenComp->invenItemArr[i]);
				WrapBox->AddChildToWrapBox(Slots[i]);
			}
		}
	}


	FText currslotName;
	switch (currinventype)
	{
	case EItemType::Consum:
		currslotName = FText::FromString(TEXT("소비창"));
		break;
	case EItemType::Outfit:
		currslotName = FText::FromString(TEXT("장비창"));
		break;
	case EItemType::Quest:
		currslotName = FText::FromString(TEXT("퀘스트창"));
		break;
	case EItemType::Food:
		currslotName = FText::FromString(TEXT("음식창"));
		break;
	}
	CurrSlot_Text->SetText(currslotName);
}


void UInventoryWG::Setinventory()
{
	Money_Text->SetText(FText::AsNumber(InvenComp->Money));
	Text_Count->SetText(FText::AsNumber(InvenComp->CountItem()));
	ChangeInven(currinventype);
}

void UInventoryWG::ClearOverlay()
{
	itemDescription->ClearChildren();
	Overlay_Use->ClearChildren();
}

