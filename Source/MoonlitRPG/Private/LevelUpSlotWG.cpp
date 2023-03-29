// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpSlotWG.h"
#include <UMG/Public/Components/CanvasPanel.h>
#include <UMG/Public/Components/HorizontalBox.h>
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Image.h>
#include "NeedItemSelectWG.h"
#include "WeaponLevelUpWG.h"

void ULevelUpSlotWG::ButtonBinding()
{
	ItemButton->OnPressed.AddUniqueDynamic(this, &ULevelUpSlotWG::SlotClicked);
}

void ULevelUpSlotWG::SlotClicked()
{
	if (!NeedSelectWG->IsInViewport())
	{
		NeedSelectWG->AddToViewport();
	}
}

void ULevelUpSlotWG::UpdateSlot(FInvenItem invenitem)
{
	Super::UpdateSlot(invenitem);
	Panal_Level->SetVisibility(ESlateVisibility::Visible);
	TB_WeaponLevel->SetText(FText::AsNumber(invenData.invenitem.WeaponData.Level));
	isFill = true;
	WeaponLevelUpWG->UpdateUseMoney();
}

void ULevelUpSlotWG::ResetSlot()
{
	Panal_Level->SetVisibility(ESlateVisibility::Hidden);
	ItemImage->SetBrushFromTexture(resetImage);
	SlotBG->SetBrushFromTexture(DataManager->ItemGradeData[0].Slotlmage, true);
	isFill =false;


	WeaponLevelUpWG->SelectListBox->RemoveChild(this);
	WeaponLevelUpWG->SelectListBox->AddChild(this);

}

