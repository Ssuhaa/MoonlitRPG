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
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
	ButtonBinding();
}

void UWeaponUpgradeWG::ButtonBinding()
{
	Button_Upgrade->OnPressed.AddUniqueDynamic(this, &UWeaponUpgradeWG::UpGrade);
}

void UWeaponUpgradeWG::SetUpGradeWG(FinvenData* InvenData)
{
	inventoryData = InvenData;
	TB_CurrLevel->SetText(FText::AsNumber(InvenData->invenitem->WeaponData.Level));
	TB_CurrLevel1->SetText(FText::AsNumber(InvenData->invenitem->WeaponData.Level));
	TB_BeforeMaxLevel->SetText(FText::AsNumber(InvenData->invenitem->WeaponData.MaxLevel));
	TB_AfterMaxLevel->SetText(FText::AsNumber(InvenData->invenitem->WeaponData.MaxLevel + 10));
	TB_UpGradeMoney->SetText(FText::AsNumber(InvenData->UpGradeMoneyData->UpgradeNeedMoney));
	TB_CurrPower->SetText(FText::AsNumber(InvenData->invenitem->WeaponData.CurrPower));
	TB_UpgradePower->SetText(FText::AsNumber(InvenData->invenitem->WeaponData.CurrPower + InvenData->itemGradeData->PlusPower * 9));
	
	for (int32 i = 0; i < UpgradeSlots.Num(); i++)
	{
		UpgradeSlots[i]->Index = i;
		UpgradeSlots[i]->UpdateSlot(InvenData->invenitem);
		NeedItemList->AddChildToHorizontalBox(UpgradeSlots[i]);
	}
}


void UWeaponUpgradeWG::UpGrade()
{
	if (player != nullptr)
	{
		if (isHaveAllNeedItem())
		{
			int32 index = player->InvenComp->FindItem(inventoryData->invenitem);
			if (index < 0) return;

			bool result = player->InvenComp->invenItemArr[index].Upgrade(&player->InvenComp->Money, isHaveAllNeedItem(), DataManager);
			if (result)
			{
				SendToUsedItem();
				OutfitWG->ReceiveUseItem(&player->InvenComp->invenItemArr[index]);
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
		player->InvenComp->MinusItemAmount(UpgradeSlots[i]->UpgradeItem.ItemInfoIndex, UpgradeSlots[i]->UseAmount);
	}
}
