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
#include "OutfitWG.h"



UInventoryWG::UInventoryWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UInventorySlotWG> tempslot(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_InvenSlot.BP_InvenSlot_C'"));
	if (tempslot.Succeeded())
	{
		WGFactory.Add(tempslot.Class); //0번 slot 위젯
	}
	ConstructorHelpers::FClassFinder <UinventoryUseButton> tempButton(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_InvenUseButton.BP_InvenUseButton_C'"));
	if (tempButton.Succeeded())
	{
		WGFactory.Add(tempButton.Class) ; //1번 사용하기 버튼 위젯
	}
	ConstructorHelpers::FClassFinder <UItemDescriptionWG> tempDesrip(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_ItemDescription.BP_ItemDescription_C'"));
	if (tempDesrip.Succeeded())
	{
		WGFactory.Add(tempDesrip.Class); //2번 아이템 정보 위젯
	}
	ConstructorHelpers::FClassFinder <UFoodPopup> tempfoodPop(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_FoodPopup.WG_FoodPopup_C'"));
	if (tempfoodPop.Succeeded())
	{
		WGFactory.Add(tempfoodPop.Class); //3번 음식 팝업위젯
	}
	ConstructorHelpers::FClassFinder <UOutfitWG> tempOutfit(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Outfit.WG_Outfit_C'"));
	if (tempOutfit.Succeeded())
	{
		WGFactory.Add(tempOutfit.Class); //4번 장비창 위젯
	}
	ButtonWG = CreateWidget<UinventoryUseButton>(GetWorld(), WGFactory[1]);
	Description = CreateWidget<UItemDescriptionWG>(GetWorld(), WGFactory[2]);
	
	FoodPopup = CreateWidget<UFoodPopup>(GetWorld(), WGFactory[3]);
	OutfitWG = CreateWidget<UOutfitWG>(GetWorld(), WGFactory[4]);

	for (int32 i = 0; i < 100; i++)
	{
		UInventorySlotWG* currslot = CreateWidget<UInventorySlotWG>(GetWorld(), WGFactory[0]);
		Slots.Add(currslot);
	}

}

void UInventoryWG::NativeConstruct() //위젯이 뷰포트에 보여질 때
{
	Super::NativeConstruct();
	ButtonBinding();
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		Player->bInventoryOpen = true;
	}

	Setinventory();
	
	FoodPopup->invenWG = this;
}

void UInventoryWG::NativeDestruct()
{
	Super::NativeDestruct();

	SetSelectSlot.Clear();

}

void UInventoryWG::ButtonBinding()
{

	SetSelectSlot.AddUObject(Description, &UItemDescriptionWG::ReceiveSelectSlotData);
	SetSelectSlot.AddUObject(FoodPopup, &UFoodPopup::ReceiveSelectSlotData);
	SetSelectSlot.AddUObject(OutfitWG, &UOutfitWG::ReceiveSelectSlotData);
	

	Button_Close->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedCloseButton);
	Button_Consum->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedConsum);
	Button_Outfit->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedOutfit);
	Button_Food->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedFood);
	Button_Quest->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedQuest);
	Button_Right->OnPressed.AddUniqueDynamic(this, &UInventoryWG::RightMenu);
	Button_Left->OnPressed.AddUniqueDynamic(this, &UInventoryWG::LeftMenu);

	ButtonWG->Button_Use->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedUseButton);

}



//아이템을 클릭했을 때 
void UInventoryWG::ItemSlotClicked(int32 slotindex) 
{	
	//선택된 슬랏의 정보를 보낸다.
	if (SetSelectSlot.IsBound())
	{
		SetSelectSlot.Broadcast(Slots[slotindex]);
	}

	//아이템 설명 창이 뜬다.
	itemDescription->AddChild(Description);

	//선택한 아이템의 타입에 따라 사용 버튼이 뜬다.
	SelectItemType = Slots[slotindex]->invenInfo.iteminfomation.itemType;
	Overlay_Use->ClearChildren();
	switch (SelectItemType)
	{
	case EItemType::Food:
		ButtonWG->SetText(TEXT("사용하기"));
		Overlay_Use->AddChildToOverlay(ButtonWG);
		break;
	case EItemType::Outfit:
		ButtonWG->SetText(TEXT("착용하기"));
		Overlay_Use->AddChildToOverlay(ButtonWG);
		break;
	}

}



void UInventoryWG::ClickedUseButton()
{
	switch (SelectItemType)
	{
	case EItemType::Outfit:
		OutfitWG->AddToViewport();
		break;
	case EItemType::Food:
		FoodPopup->AddToViewport();
		break;
	}
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

void UInventoryWG::ClickedCloseButton()
{
	RemoveInventory();
}


void UInventoryWG::Setinventory()
{
	Money_Text->SetText(FText::AsNumber(InvenComp->Money));
	Text_Count->SetText(FText::AsNumber(InvenComp->CountItem()));
	ChangeInven(currinventype);
}

void UInventoryWG::ChangeInven(EItemType ChangeInvenType)
{
	currinventype = ChangeInvenType;
	ClearInvenWGChild();

	WrapBox->ClearChildren();
	TArray<FInvenItem> FindArr = InvenComp->FindAllItemsType(currinventype);
	if (!FindArr.IsEmpty())
	{
		for (int32 i = 0; i < FindArr.Num(); i++)
		{
			if (!Slots.IsValidIndex(i)) //슬랏이 모자란다면 슬랏을 생성하자.
			{
				UInventorySlotWG* currslot = CreateWidget<UInventorySlotWG>(GetWorld(), WGFactory[0]);
				Slots.Add(currslot);
			}
			Slots[i]->invenWG = this;
			Slots[i]->Slotindex = i;
			Slots[i]->UpdateSlot(FindArr[i]);
			WrapBox->AddChildToWrapBox(Slots[i]);
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


void UInventoryWG::ClearInvenWGChild()
{
	itemDescription->ClearChildren();
	Overlay_Use->ClearChildren();
}

void UInventoryWG::RemoveInventory()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	FoodPopup->RemoveFromParent();
	OutfitWG->RemoveFromParent(); 
	Player->bInventoryOpen = false;

	RemoveFromParent();
}
