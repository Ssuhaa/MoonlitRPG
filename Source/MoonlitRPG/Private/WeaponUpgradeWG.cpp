// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUpgradeWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include "InventoryComponent.h"
#include <UMG/Public/Components/Button.h>
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "InventorySlotWG.h"



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
}


void UWeaponUpgradeWG::UpGrade()
{
	if (player != nullptr)
	{
		SelectedSlotItem.weaponinfomaiton.Upgrade(&player->InvenComp->Money, true);
	}
	SetUpGradeWG(SelectedSlotItem);
}
