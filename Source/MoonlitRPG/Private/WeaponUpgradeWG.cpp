// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUpgradeWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/HorizontalBox.h>
#include "InventoryComponent.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "InventorySlotWG.h"
#include "UpgradeSlotWG.h"
#include "OutfitWG.h"



UWeaponUpgradeWG::UWeaponUpgradeWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUpgradeSlotWG> tempSlot(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_UpgradeSlot.BP_UpgradeSlot_C'"));
	if (tempSlot.Succeeded())
	{
		WGFactory.Add(tempSlot.Class);
	}
	for (int32 i = 0; i < 3; i++)
	{
		UUpgradeSlotWG* currSlot = CreateWidget<UUpgradeSlotWG>(GetWorld(), WGFactory[0]);
		UpgradeSlots.Add(currSlot);
	}
		
}

void UWeaponUpgradeWG::NativeConstruct()
{
	Super::NativeConstruct();
	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	ButtonBinding();
}

void UWeaponUpgradeWG::ButtonBinding()
{
	Button_Upgrade->OnPressed.AddUniqueDynamic(this, &UWeaponUpgradeWG::UpGrade);
}

void UWeaponUpgradeWG::SetUpGradeWG(FInvenItem SelectSlotItem)
{
	SelectedSlotItem = SelectSlotItem;
	FWeaponinfo WeaponData = SelectedSlotItem.weaponinfomaiton;
	TB_CurrLevel->SetText(FText::AsNumber(WeaponData.Level));
	TB_CurrLevel1->SetText(FText::AsNumber(WeaponData.Level));
	TB_AfterMaxLevel->SetText(FText::AsNumber(WeaponData.MaxLevel + 10));
	TB_BeforeMaxLevel->SetText(FText::AsNumber(WeaponData.MaxLevel));
	TB_UpGradeMoney->SetText(FText::AsNumber(WeaponData.UpGradeMoney));
	TB_CurrPower->SetText(FText::AsNumber(WeaponData.Power));
	TB_UpgradePower->SetText(FText::AsNumber(WeaponData.Power + WeaponData.PlusPower * 9));
	
	for (int32 i = 0; i < UpgradeSlots.Num(); i++)
	{
		UpgradeSlots[i]->Index = i;
		UpgradeSlots[i]->UpdateSlot(SelectedSlotItem);
		NeedItemList->AddChildToHorizontalBox(UpgradeSlots[i]);
		
	}
}


void UWeaponUpgradeWG::UpGrade()
{
	if (player != nullptr)
	{
		if (isHaveAllNeedItem())
		{
			int32 index = player->InvenComp->FindItem(SelectedSlotItem);
			if (index < 0) return;
			bool result = player->InvenComp->invenItemArr[index].weaponinfomaiton.Upgrade(&player->InvenComp->Money, isHaveAllNeedItem());
			if (result)
			{
				
				SelectedSlotItem = player->InvenComp->invenItemArr[index];
				SendToUsedItem();
				OutfitWG->ReceiveUseItem(SelectedSlotItem);

			}

		}
	}
}

bool UWeaponUpgradeWG::isHaveAllNeedItem()
{
	for (int32 i = 0; i < UpgradeSlots.Num(); i++)
	{
		if(!UpgradeSlots[i]->isHaveAmount) return false;
	}
	return true;
}

void UWeaponUpgradeWG::SendToUsedItem()
{
	
	for (int32 i = 0; i < UpgradeSlots.Num(); i++)
	{
		player->InvenComp->MinusItemAmount(UpgradeSlots[i]->UpgradeItem, UpgradeSlots[i]->UseAmount);
	}
}
