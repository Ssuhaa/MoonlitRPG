// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include "InventorySlotWG.h"
#include <UMG/Public/Components/CanvasPanel.h>
#include <UMG/Public/Components/Border.h>
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include "InventoryComponent.h"


void UItemDescriptionWG::SetDescription()
{
	ItemImage->SetBrushFromTexture(inventoryData.iteminfo.itemImage);
	ItemName->SetText(FText::FromString(inventoryData.iteminfo.ItemName));
	Description->SetText(FText::FromString(inventoryData.iteminfo.itemDescription));
	BG->SetBrushFromTexture(inventoryData.itemGradeData.DiscriptionImage, true);

	FText weaponType;
	int32 currPower;
	switch (inventoryData.iteminfo.itemType)
	{
	case EItemType::Outfit:

		Panel_Weapon->SetVisibility(ESlateVisibility::Visible);
		Panel_Food->SetVisibility(ESlateVisibility::Hidden);

		switch (int32(inventoryData.Weaponinfo.WeaponType))
		{
		case 0:
			weaponType = FText::FromString(TEXT("두손 검"));
			break;
		case 1:
			weaponType = FText::FromString(TEXT("단 검"));
			break;
		case 2:
			weaponType = FText::FromString(TEXT("활"));
			break;
		}

		Text_WeaponType->SetText(weaponType);
		currPower = inventoryData.invenitem.WeaponData.CurrPower;
		Text_Power->SetText(FText::AsNumber(currPower));
		ShowEquip();

		break;

	case EItemType::Food:

		Panel_Food->SetVisibility(ESlateVisibility::Visible);
		Panel_Weapon->SetVisibility(ESlateVisibility::Hidden);
		Text_Heal->SetText(FText::AsNumber(inventoryData.iteminfo.HealAmount));
		break;
	case EItemType::Consum:
		Panel_Food->SetVisibility(ESlateVisibility::Hidden);
		Panel_Weapon->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EItemType::Etc:
		Panel_Food->SetVisibility(ESlateVisibility::Hidden);
		Panel_Weapon->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EItemType::Quest:
		Panel_Food->SetVisibility(ESlateVisibility::Hidden);
		Panel_Weapon->SetVisibility(ESlateVisibility::Hidden);
		break;
	}

}


void UItemDescriptionWG::ShowEquip()
{
	int32 value = Player->InvenComp->CheckWeaponisEquip(); //착용하고 있는 아이템 체크
	if (value > -1) //있으면
	{
		if (Player->InvenComp->invenItemArr[value] == inventoryData.invenitem) //나온것이 현재 장비창에 띄운 인포랑 같은지
		{
			Border_Equip->SetVisibility(ESlateVisibility::Visible);
			return;
		}
	}

	Border_Equip->SetVisibility(ESlateVisibility::Hidden);
}

void UItemDescriptionWG::ReceiveSelectSlotData(FinvenData invenData)
{
	inventoryData = invenData;
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	SetDescription();
}

