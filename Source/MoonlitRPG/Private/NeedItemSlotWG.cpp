// Fill out your copyright notice in the Description page of Project Settings.


#include "NeedItemSlotWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/CanvasPanel.h>
#include "LevelUpSlotWG.h"

void UNeedItemSlotWG::SlotClicked()
{
	if (!isSelect)
	{	
		for (int32 i = 0; i < LevelUpSlots->Num(); i++)
		{			
			if (((*LevelUpSlots)[i])->isFill == false)
			{
				((*LevelUpSlots)[i])->UpdateSlot(invenInfo);
				Selectindex = i;
				isSelect = true;
				pSelect = ((*LevelUpSlots)[i]);
				return;
			}
			
		}
	}
	else
	{
		pSelect->ResetSlot();
		LevelUpSlots->Remove(pSelect);
		LevelUpSlots->Add(pSelect);
		pSelect = nullptr;
		isSelect = false;

	}

}

void UNeedItemSlotWG::UpdateSlot(FInvenItem* invenData)
{	
	invenInfo = invenData;
	Panal_Level->SetVisibility(ESlateVisibility::Visible);
	ItemImage->SetBrushFromTexture(invenInfo->iteminfomation.itemImage);
	int32 BGindex = int32(invenInfo->iteminfomation.itemgrade);
	SlotBG->SetBrushFromTexture(BGarray[BGindex], true);
	TB_WeaponLevel->SetText(FText::AsNumber(invenInfo->weaponinfomaiton.Level));
}

