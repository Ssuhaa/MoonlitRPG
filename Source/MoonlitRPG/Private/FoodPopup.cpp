// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodPopup.h"
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/TextBlock.h>
#include "SH_Player.h"
#include <UMG/Public/Components/ProgressBar.h>
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Image.h>
#include "InventoryComponent.h"
#include "InventoryWG.h"
#include "InventorySlotWG.h"



void UFoodPopup::ButtonBinding()
{
	Button_Cancel->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedCancel);
	Button_Use->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedUse);
	Button_Close->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedClose);
	Button_Plus->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedPlus);
	Button_Minus->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedMinus);
}


void UFoodPopup::SetFoodPop(FInvenItem selectitem)
{
	iteminfo = selectitem;
	ItemImage->SetBrushFromTexture(selectitem.iteminfomation.itemImage);
	Text_Amount->SetText(FText::AsNumber(CurrAmount));
	UpdateHPBar();
}


void UFoodPopup::OnClickedCancel()
{
	RemoveFromParent();
}

void UFoodPopup::OnClickedUse()
{
	if (Player != nullptr)
	{
		//HP 회복 호출
		Player->HealPlayer(iteminfo.iteminfomation.HealAmount*CurrAmount);
		UpdateHPBar();
		int32 result = Player->InvenComp->PlusMinusItemAmount(iteminfo.iteminfomation, - CurrAmount);
		invenWG->Setinventory();
		CurrAmount = 1;
		if(result <= 0)
		{
			invenWG->ClearOverlay();
			RemoveFromParent();
		}
		//hp 다찼을때 사용할지 예외처리
	}

}

void UFoodPopup::OnClickedClose()
{
	RemoveFromParent();
}

void UFoodPopup::OnClickedPlus()
{
	CurrAmount++;
	CurrAmount = FMath::Clamp(CurrAmount, 1, iteminfo.itemAmount);
	Text_Amount->SetText(FText::AsNumber(CurrAmount));
	UpdateHPBar();
}

void UFoodPopup::OnClickedMinus()
{
	CurrAmount--;
	CurrAmount = FMath::Clamp(CurrAmount, 1, iteminfo.itemAmount);
	Text_Amount->SetText(FText::AsNumber(CurrAmount));
	UpdateHPBar();
}

void UFoodPopup::UpdateHPBar()
{
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		HP = Player->PlayercurrHP / Player->PlayerTotalHP;
		CurrHPBar->SetPercent(HP);
		HealHP = (iteminfo.iteminfomation.HealAmount * CurrAmount) / Player->PlayerTotalHP;
		HealHPBar->SetPercent(HP + HealHP);
	}
}
