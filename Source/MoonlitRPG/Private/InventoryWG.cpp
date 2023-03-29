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
	ButtonWG = CreateWGClass<UinventoryUseButton>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_InvenUseButton.BP_InvenUseButton_C'"));
	Description = CreateWGClass<UItemDescriptionWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_ItemDescription.BP_ItemDescription_C'"));
	FoodPopup = CreateWGClass<UFoodPopup>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_FoodPopup.WG_FoodPopup_C'"));
	OutfitWG = CreateWGClass<UOutfitWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Outfit.WG_Outfit_C'"));

	for (int32 i = 0; i < 100; i++)
	{
		UInventorySlotWG* currslot = CreateWGClass<UInventorySlotWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_InvenSlot.BP_InvenSlot_C'"));
		Slots.Add(currslot);
	}
}

template<typename T>
T* UInventoryWG::CreateWGClass(FString path)
{
	TSubclassOf<T> WGFactory;
	ConstructorHelpers::FClassFinder<T> tempWG(*path);
	if (tempWG.Succeeded())
	{
		WGFactory = tempWG.Class;
	}
	return CreateWidget<T>(GetWorld(), WGFactory);
}


void UInventoryWG::NativeConstruct() //위젯이 뷰포트에 보여질 때
{
	Super::NativeConstruct();

	ButtonBinding();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	//캐싱
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		Player->bUIOpen = true;
	}

	Setinventory();
	
	FoodPopup->invenWG = this;
}

void UInventoryWG::NativeDestruct()
{
	Super::NativeDestruct();
	
	SendInvenData.Clear();
}

void UInventoryWG::ButtonBinding()
{
	SendInvenData.AddUObject(Description, &UItemDescriptionWG::ReceiveSelectSlotData);
	SendInvenData.AddUObject(FoodPopup, &UFoodPopup::ReceiveSelectSlotData);
	SendInvenData.AddUObject(OutfitWG, &UOutfitWG::ReceiveSelectSlotData);
	
	Button_Close->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedCloseButton);
	Button_Consum->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedConsum);
	Button_Outfit->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedOutfit);
	Button_Food->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedFood);
	Button_ETC->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedETC);
	Button_Quest->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedQuest);
	Button_Right->OnPressed.AddUniqueDynamic(this, &UInventoryWG::RightMenu);
	Button_Left->OnPressed.AddUniqueDynamic(this, &UInventoryWG::LeftMenu);
	ButtonWG->Button_Use->OnPressed.AddUniqueDynamic(this, &UInventoryWG::ClickedUseButton);
}



//아이템을 클릭했을 때 
void UInventoryWG::ItemSlotClicked(FinvenData invenData)
{	
	//선택된 슬랏의 정보를 보낸다.
	if (SendInvenData.IsBound())
	{
		SendInvenData.Broadcast(invenData);
	}

	//아이템 설명 창이 뜬다.
	itemDescription->ClearChildren();
	itemDescription->AddChild(Description);

	//선택한 아이템의 타입에 따라 사용 버튼이 뜬다.
	Overlay_Use->ClearChildren();
	switch (invenData.iteminfo.itemType)
	{
	case EItemType::Food:
		ButtonWG->SetText(TEXT("사용하기"));
		Overlay_Use->AddChildToOverlay(ButtonWG);
		break;
	case EItemType::Outfit:
		ButtonWG->SetText(TEXT("상세보기"));
		Overlay_Use->AddChildToOverlay(ButtonWG);
		break;
	}

}



void UInventoryWG::ClickedUseButton()
{
	switch (currinventype)
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

void UInventoryWG::ClickedETC()
{
	ChangeInven(EItemType::Etc);
}

void UInventoryWG::LeftMenu()
{
	int32 index = int32(currinventype);
	index--;
	if (index < 0)
	{
		index = int32(EItemType::None) - 1;
	}
	ChangeInven(EItemType(index));
}

void UInventoryWG::RightMenu()
{
	int32 index = int32(currinventype);
	index++;
	if (index > int32(EItemType::None) - 1)
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
	TArray<FInvenItem> FindArr = InvenComp->FindAllItemsType(ChangeInvenType);
	if (!FindArr.IsEmpty())
	{
		for (int32 i = 0; i < FindArr.Num(); i++)
		{
			if (!Slots.IsValidIndex(i)) //슬랏이 모자란다면 슬랏을 생성하자.
			{
				UInventorySlotWG* currslot = CreateWGClass<UInventorySlotWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_InvenSlot.BP_InvenSlot_C'"));
				Slots.Add(currslot);
			}
			Slots[i]->InvenWG = this;
			Slots[i]->UpdateSlot(FindArr[i]);
			WrapBox->AddChildToWrapBox(Slots[i]);
		}
	}
	if (WrapBox->GetChildrenCount() != 0)
	{
		UInventorySlotWG* First = Cast<UInventorySlotWG>(WrapBox->GetChildAt(0));
		ItemSlotClicked(First->invenData);
	}

	FText currslotName;
	switch (ChangeInvenType)
	{
	case EItemType::Consum:
		currslotName = FText::FromString(TEXT("소비 창"));
		break;
	case EItemType::Outfit:
		currslotName = FText::FromString(TEXT("장비 창"));
		break;
	case EItemType::Quest:
		currslotName = FText::FromString(TEXT("퀘스트 창"));
		break;
	case EItemType::Food:
		currslotName = FText::FromString(TEXT("음식 창"));
		break;
	case EItemType::Etc:
		currslotName = FText::FromString(TEXT("기타 창"));
	}
	CurrSlot_Text->SetText(currslotName);
}


void UInventoryWG::ClearInvenWGChild()
{
	itemDescription->ClearChildren();
	Overlay_Use->ClearChildren();
	FoodPopup->RemoveFromParent();
	OutfitWG->RemoveFromParent();
}

void UInventoryWG::RemoveInventory()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	ClearInvenWGChild();
	Player->bUIOpen = false;

	RemoveFromParent();
}
