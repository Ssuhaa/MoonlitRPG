// Fill out your copyright notice in the Description page of Project Settings.


#include "OutfitWG.h"
#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/ScaleBox.h>
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/TextBlock.h>
#include "InventorySlotWG.h"
#include <UMG/Public/Components/Border.h>
#include <UMG/Public/Components/CanvasPanel.h>
#include "WeaponUpgradeWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>

UOutfitWG::UOutfitWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UItemDescriptionWG> tempDesrip(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_ItemDescription.BP_ItemDescription_C'"));
	if (tempDesrip.Succeeded())
	{
		WGFactory.Add(tempDesrip.Class);
	}
	ConstructorHelpers::FClassFinder <UWeaponUpgradeWG> tempUpgrade(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_WeaponUpgrade.WG_WeaponUpgrade_C'"));
	if (tempUpgrade.Succeeded())
	{
		WGFactory.Add(tempUpgrade.Class);
	}
	description = CreateWidget<UItemDescriptionWG>(GetWorld(), WGFactory[0]);
	UpgradeWG = CreateWidget<UWeaponUpgradeWG>(GetWorld(), WGFactory[1]);
}

void UOutfitWG::NativeConstruct()
{
	Super::NativeConstruct();
	ReinforceSwitch(EEquipmentState::Detail);
	
}

void UOutfitWG::ButtonBinding()
{
	Button_Detail->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedDetail);
	Button_LevelUp->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedLevelUp);
	Button_Upgrade->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedUpgrade);
	Button_Wearing->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedWearing);
	Button_Back->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedBack);
	UpgradeWG->ButtonBinding();
	UpgradeWG->Button_Upgrade->OnPressed.AddDynamic(this, &UOutfitWG::UpdateMoney);
}


void UOutfitWG::OnclickedDetail()
{
	ReinforceSwitch(EEquipmentState::Detail);
}
void UOutfitWG::OnclickedLevelUp()
{
	ReinforceSwitch(EEquipmentState::LevelUp);
}

void UOutfitWG::OnclickedUpgrade()
{
	ReinforceSwitch(EEquipmentState::Upgrade);
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

void UOutfitWG::SetOutfitWG(class UInventorySlotWG* SelectSlot)
{
	description->SetDescription(SelectSlot);
	itemDescription->AddChild(description);
	UpgradeWG->SetUpGradeWG(SelectSlot->selectiteminfo);

	SelectInveninfo = SelectSlot->selectiteminfo;

	UpdateMoney();

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

	FText itemname = FText::FromString(SelectInveninfo->iteminfomation.ItemName);
	Text_WeaponName->SetText(itemname);


	SelectInveninfo->weaponinfomaiton.Level == SelectInveninfo->weaponinfomaiton.MaxLevel ? ButtonSwitch(true) : ButtonSwitch(false);
	
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

void UOutfitWG::ButtonSwitch(bool isMaxLevel)
{
	if (isMaxLevel)
	{
		Button_Upgrade->SetVisibility(ESlateVisibility::Visible);
		Button_LevelUp->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Button_LevelUp->SetVisibility(ESlateVisibility::Visible);
		Button_Upgrade->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOutfitWG::ReinforceSwitch(EEquipmentState state)
{
	OutfitState = state;
	switch (OutfitState)
	{
	case EEquipmentState::Detail:
		SelectInveninfo->weaponinfomaiton.isEquip ? WearingSwitch(true) : WearingSwitch(false);
		Panel_Detail->SetVisibility(ESlateVisibility::Visible);
		Panel_Reinforce->ClearChildren();
		break;
	case EEquipmentState::Upgrade:
		Panel_Reinforce->ClearChildren();
		Panel_Reinforce->AddChildToCanvas(UpgradeWG);
		Panel_Detail->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EEquipmentState::LevelUp:
		Panel_Reinforce->ClearChildren();
		//레벨업 위젯 필요
		Panel_Detail->SetVisibility(ESlateVisibility::Hidden);
		break;
	}
}

void UOutfitWG::UpdateMoney()
{
	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (player != nullptr)
	{
		Text_Money->SetText(FText::AsNumber(player->InvenComp->Money));
	}
	SelectInveninfo->weaponinfomaiton.Level == SelectInveninfo->weaponinfomaiton.MaxLevel ? ButtonSwitch(true) : ButtonSwitch(false);
	ReinforceSwitch(EEquipmentState::LevelUp);
}

