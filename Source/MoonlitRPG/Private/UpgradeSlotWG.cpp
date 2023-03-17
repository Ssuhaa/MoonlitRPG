// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeSlotWG.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBlock.h>
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>


void UUpgradeSlotWG::UpdateSlot(FInvenItem invenData)
{
	invenInfo = invenData;
	int32 UpgradeCount =  invenInfo.weaponinfomaiton.UpgradeCount;
	if (invenInfo.weaponinfomaiton.UpgradeItemList.IsValidIndex(UpgradeCount))
	{
		FUpgradeNeedItem item = invenInfo.weaponinfomaiton.UpgradeItemList[UpgradeCount].UpgradeNeedItem[Index];
		ItemImage->SetBrushFromTexture(item.itemImage);
		int32 BGindex = int32(item.itemgrade);
		SlotBG->SetBrushFromTexture(BGarray[BGindex], true);
		UseAmount = item.UseAmont;
		TB_NeedAmount->SetText(FText::AsNumber(UseAmount));
		player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
		if (player != nullptr)
		{
			int32 findindex = player->InvenComp->FindItem(item.ItemName);
			int32 currAmount;
			if (findindex > -1)
			{
				UpgradeItem = player->InvenComp->invenItemArr[findindex];
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
