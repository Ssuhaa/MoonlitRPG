// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include "InventorySlotWG.h"
#include <UMG/Public/Components/CanvasPanel.h>
#include <UMG/Public/Components/Border.h>
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"


UItemDescriptionWG::UItemDescriptionWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/DescriptionBG_Common.DescriptionBG_Common'"));
	if (tempT.Succeeded())
	{
		BGarray.Add(tempT.Object); //0번 Common
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT1(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/DescriptionBG_RA.DescriptionBG_RA'"));
	if (tempT1.Succeeded())
	{
		BGarray.Add(tempT1.Object);	//1번 Rare
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT2(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/DescriptionUinque_Rare.DescriptionUinque_Rare'"));
	if (tempT2.Succeeded())
	{
		BGarray.Add(tempT2.Object); //2번 Uinque
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT3(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/DescriptionBG_Legendary.DescriptionBG_Legendary'"));
	if (tempT3.Succeeded())
	{
		BGarray.Add(tempT3.Object);	//3번 Legendary
	}
}


void UItemDescriptionWG::ReceiveSelectSlotitemData(FInvenItem SelectSlotItem)
{
	SelectedSlotItem = SelectSlotItem;
	UpdateDescription();
}


void UItemDescriptionWG::ReceiveSelectSlotData(class UInventorySlotWG* SelectSlot)
{
	SelectedSlot = SelectSlot;
	SelectedSlotItem = SelectedSlot->invenInfo;
	UpdateDescription();
}

void UItemDescriptionWG::UpdateDescription()
{	
	ItemImage->SetBrushFromTexture(SelectedSlotItem.iteminfomation.itemImage);
	ItemName->SetText(FText::FromString(SelectedSlotItem.iteminfomation.ItemName));
	Description->SetText(FText::FromString(SelectedSlotItem.iteminfomation.itemDescription));

	int32 BGindex = int32(SelectedSlotItem.iteminfomation.itemgrade);
	BG->SetBrushFromTexture(BGarray[BGindex], true);

	if(SelectedSlotItem.weaponinfomaiton.WeaponType != EWeaponType::None)
	{
		Panel_Weapon->SetVisibility(ESlateVisibility::Visible);
		Panel_Food->SetVisibility(ESlateVisibility::Hidden);
		int32 Typeindex = int32(SelectedSlotItem.weaponinfomaiton.WeaponType);
		FText weaponType;
		
		ASH_Player* player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
		int32 value = player->InvenComp->CheckWeaponisEquip(); //착용하고 있는 아이템 체크
		if (value > -1) //있으면
		{
			if (player->InvenComp->invenItemArr[value] == SelectedSlotItem) //나온것이 현재 장비창에 띄운 인포랑 같은지
			{
				Border_Equip->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Border_Equip->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			Border_Equip->SetVisibility(ESlateVisibility::Hidden);
		}


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
		int32 currPower = SelectedSlotItem.weaponinfomaiton.Power;
		Text_Power->SetText(FText::AsNumber(currPower));
	}
	else
	{
		if (SelectedSlotItem.iteminfomation.itemType == EItemType::Food)
		{
			Panel_Food->SetVisibility(ESlateVisibility::Visible);
			Text_Heal->SetText(FText::AsNumber(SelectedSlotItem.iteminfomation.HealAmount));
		}
		Panel_Weapon->SetVisibility(ESlateVisibility::Hidden);
	}

}

