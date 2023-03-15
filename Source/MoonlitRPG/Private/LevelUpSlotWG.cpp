// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpSlotWG.h"
#include <UMG/Public/Components/CanvasPanel.h>
#include "NeedItemSelectWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Image.h>
#include "WeaponLevelUpWG.h"
#include <UMG/Public/Components/HorizontalBox.h>


void ULevelUpSlotWG::SlotClicked()
{
	if (!NeedSelectWG->IsInViewport())
	{
		NeedSelectWG->AddToViewport();
	}
}

void ULevelUpSlotWG::UpdateSlot(FInvenItem* invenData)
{
	invenInfo = invenData;
	Panal_Level->SetVisibility(ESlateVisibility::Visible);
	ItemImage->SetBrushFromTexture(invenInfo->iteminfomation.itemImage);
	int32 BGindex = int32(invenInfo->iteminfomation.itemgrade);
	SlotBG->SetBrushFromTexture(BGarray[BGindex], true);
	TB_WeaponLevel->SetText(FText::AsNumber(invenInfo->weaponinfomaiton.Level));
	isFill = true;
	WeaponLevelUpWG->UpdateUseMoney();
}

void ULevelUpSlotWG::ResetSlot()
{
	Panal_Level->SetVisibility(ESlateVisibility::Hidden);
	ItemImage->SetBrushFromTexture(resetImage);
	SlotBG->SetBrushFromTexture(BGarray[0], true);
	isFill =false;
	invenInfo = nullptr;

	WeaponLevelUpWG->SelectListBox->RemoveChild(this);
	WeaponLevelUpWG->SelectListBox->AddChild(this);

}

