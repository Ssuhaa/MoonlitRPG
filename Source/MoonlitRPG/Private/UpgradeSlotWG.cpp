// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeSlotWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include "InventoryComponent.h"


void UUpgradeSlotWG::UpdateSlot(FInvenItem invenitem)
{
	Super::UpdateSlot(invenitem);
	int32 UpgradeCount = invenData.invenitem.WeaponData.UpgradeCount;
	if (invenData.Weaponinfo.UpgradeItemList.IsValidIndex(UpgradeCount))
	{
		FUpgradeNeedItem item = invenData.Weaponinfo.UpgradeItemList[UpgradeCount].UpgradeNeedItem[Index];
		FIteminfo info = DataManager->itemList[item.iteminfoIndex];
		ItemImage->SetBrushFromTexture(info.itemImage);
		FItemGradeData GradeData = DataManager->ItemGradeData[int32(info.itemgrade)];
		SlotBG->SetBrushFromTexture(GradeData.Slotlmage, true);
		UseAmount = item.UseAmont;
		TB_NeedAmount->SetText(FText::AsNumber(UseAmount));
		Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
		if (Player != nullptr)
		{
			int32 findindex = Player->InvenComp->FindItem(item.iteminfoIndex);
			int32 currAmount;
			if (findindex > -1)
			{
				UpgradeItem = Player->InvenComp->invenItemArr[findindex];
				currAmount = UpgradeItem.itemAmount;
			}
			else
			{
				currAmount = 0;
			}

			TB_CurrAmount->SetText(FText::AsNumber(currAmount));
			if (currAmount >= item.UseAmont)
			{
				isHaveAmount = true;
				TB_CurrAmount->SetColorAndOpacity(FLinearColor::Black);
			}
			else
			{
				isHaveAmount = false;
				TB_CurrAmount->SetColorAndOpacity(FLinearColor::Red);
			}
		}
	}

}
