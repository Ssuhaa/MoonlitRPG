// Fill out your copyright notice in the Description page of Project Settings.


#include "NeedItemSlotWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/CanvasPanel.h>
#include <UMG/Public/Components/WrapBox.h>
#include <UMG/Public/Components/Border.h>
#include <UMG/Public/Components/Button.h>
#include "LevelUpSlotWG.h"
#include "NeedItemSelectWG.h"



void UNeedItemSlotWG::ButtonBinding()
{
	ItemButton->OnPressed.AddUniqueDynamic(this, &UNeedItemSlotWG::SlotClicked);
}

void UNeedItemSlotWG::SlotClicked()
{
	if (!isSelect)
	{	
		for (int32 i = 0; i < LevelUpSlots->Num(); i++)
		{			
			if ((*LevelUpSlots)[i]->isFill == false)
			{
				(*LevelUpSlots)[i]->UpdateSlot(invenInfo);
				isSelect = true;
				SelectDone->SetVisibility(ESlateVisibility::Visible);
				settingSlot = (*LevelUpSlots)[i];
				return;
			}
			
		}
	}
	else
	{
		settingSlot->ResetSlot();
		LevelUpSlots->Remove(settingSlot);
		LevelUpSlots->Add(settingSlot);
		ResetSlot();
	}

}

void UNeedItemSlotWG::UpdateSlot(FInvenItem invenData)
{	
	Super::UpdateSlot(invenData);

	TB_WeaponLevel->SetText(FText::AsNumber(invenInfo.weaponinfomaiton.Level));
}

void UNeedItemSlotWG::ResetSlot()
{
	settingSlot = nullptr;
	SelectDone->SetVisibility(ESlateVisibility::Hidden);
	isSelect = false;
}

void UNeedItemSlotWG::RemoveSlot()
{
	ResetSlot();
	NeedSelectWG->Wrap_HadWeapon->RemoveChild(this);
}

