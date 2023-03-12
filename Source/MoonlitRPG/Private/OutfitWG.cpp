// Fill out your copyright notice in the Description page of Project Settings.


#include "OutfitWG.h"
#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/ScaleBox.h>
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/TextBlock.h>
#include "InventorySlotWG.h"
#include <UMG/Public/Components/Border.h>
#include <UMG/Public/Components/CanvasPanel.h>

UOutfitWG::UOutfitWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UItemDescriptionWG> tempDesrip(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_ItemDescription.BP_ItemDescription_C'"));
	if (tempDesrip.Succeeded())
	{
		DescriptionWG = tempDesrip.Class; 
	}
	description = CreateWidget<UItemDescriptionWG>(GetWorld(), DescriptionWG);
}

void UOutfitWG::NativeConstruct()
{
	Super::NativeConstruct();
	OnclickedDetail();
}

void UOutfitWG::ButtonBinding()
{
	Button_Detail->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedDetail);
	Button_LevelUp->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedLevelUp);
	Button_Upgrade->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedUpgrade);
	Button_Wearing->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedWearing);
	Button_Back->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedBack);


}


void UOutfitWG::OnclickedDetail()
{
	Panel_Detail->SetVisibility(ESlateVisibility::Visible);
	Panel_LevelUp->SetVisibility(ESlateVisibility::Hidden);
	Panel_Upgrade->SetVisibility(ESlateVisibility::Hidden);
}
void UOutfitWG::OnclickedLevelUp()
{
	Panel_LevelUp->SetVisibility(ESlateVisibility::Visible);
	Panel_Detail->SetVisibility(ESlateVisibility::Hidden);
	Panel_Upgrade->SetVisibility(ESlateVisibility::Hidden);
}

void UOutfitWG::OnclickedUpgrade()
{
	Panel_Upgrade->SetVisibility(ESlateVisibility::Visible);
	Panel_LevelUp->SetVisibility(ESlateVisibility::Hidden);
	Panel_Detail->SetVisibility(ESlateVisibility::Hidden);
}


void UOutfitWG::OnclickedWearing()
{
	//다른 장비가 착용되었는지 확인하는 함수 필요.
	SelectInveninfo->weaponinfomaiton.isEquip = true;
	WearingSwitch(SelectInveninfo->weaponinfomaiton.isEquip);
}

void UOutfitWG::OnclickedBack()
{
	RemoveFromParent();
}



void UOutfitWG::SetOutfitWG(UInventorySlotWG* SelectSlot, int32* Money)
{
	SelectInveninfo = SelectSlot->selectiteminfo;

	WearingSwitch(SelectInveninfo->weaponinfomaiton.isEquip);
	
	Text_Money->SetText(FText::AsNumber(*Money));

	description->SetDescription(SelectSlot);
	itemDescription->AddChild(description);

	FText itemname = FText::FromString(SelectInveninfo->iteminfomation.ItemName);
	Text_WeaponName->SetText(itemname);

	int32 Typeindex = int32(SelectInveninfo->weaponinfomaiton.WeaponType);
	FText weaponType;
	switch (Typeindex)
	{
	case 1:
		weaponType = FText::FromString(TEXT("두손 검"));
		break;
	case 2:
		weaponType = FText::FromString(TEXT("단 검"));
		break;
	case 3:
		weaponType = FText::FromString(TEXT("활"));
		break;

	}
	Text_WeaponType->SetText(weaponType);

}

void UOutfitWG::WearingSwitch(bool isEquip)
{
	if (isEquip)
	{
		EquipPop->SetVisibility(ESlateVisibility::Visible);
		Button_Wearing->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		EquipPop->SetVisibility(ESlateVisibility::Hidden);
		Button_Wearing->SetVisibility(ESlateVisibility::Visible);
	}
}

