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
#include "HpFullPopup.h"


UFoodPopup::UFoodPopup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UHpFullPopup> tempfullPop(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_hpFull.WG_hpFull_C'"));
	if (tempfullPop.Succeeded())
	{
		FullPopFactory = tempfullPop.Class;
	}
	FullPopup = CreateWidget<UHpFullPopup>(GetWorld(), FullPopFactory);
}

void UFoodPopup::ButtonBinding()
{
	Button_Cancel->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedCancel);
	Button_Use->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedUse);
	Button_Close->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedClose);
	Button_Plus->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedPlus);
	Button_Minus->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedMinus);
	
	FullPopup->Button_Use->OnPressed.AddDynamic(this, &UFoodPopup::OnClickedFullPopup);
	FullPopup->ButtonBinding();
}

void UFoodPopup::SetFoodPop(UInventorySlotWG* SelectSlot)
{
	SelectedSlot = SelectSlot;
	iteminfo = SelectSlot->selectiteminfo;
	ItemImage->SetBrushFromTexture(iteminfo->iteminfomation.itemImage);
	CurrAmount = 1;
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
		if (Player->PlayercurrHP != Player->PlayerTotalHP)
		{
			//HP 회복 호출
			HealCallAndUpdatePopup();

		}
		else 
		{
			FullPopup->AddToViewport();
		}
	}
}

void UFoodPopup::OnClickedClose()
{
	RemoveFromParent();
}

void UFoodPopup::OnClickedPlus()
{
	CurrAmount++;
	CurrAmount = FMath::Clamp(CurrAmount, 1, iteminfo->itemAmount);
	Text_Amount->SetText(FText::AsNumber(CurrAmount));
	UpdateHPBar();
}

void UFoodPopup::OnClickedMinus()
{
	CurrAmount--;
	CurrAmount = FMath::Clamp(CurrAmount, 1, iteminfo->itemAmount);
	Text_Amount->SetText(FText::AsNumber(CurrAmount));
	UpdateHPBar();
}
void UFoodPopup::OnClickedFullPopup()
{
	HealCallAndUpdatePopup();
	FullPopup->RemoveFromParent();
}

void UFoodPopup::UpdateHPBar()
{
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		HP = Player->PlayercurrHP / Player->PlayerTotalHP;
		CurrHPBar->SetPercent(HP);
		HealHP = (iteminfo->iteminfomation.HealAmount * CurrAmount) / Player->PlayerTotalHP;
		HealHPBar->SetPercent(HP + HealHP);
	}
}

void UFoodPopup::HealCallAndUpdatePopup()
{
	Player->HealPlayer(iteminfo->iteminfomation.HealAmount * CurrAmount);
	UpdateHPBar();
	int32 result = Player->InvenComp->PlusMinusItemAmount(iteminfo->iteminfomation, -CurrAmount);
	invenWG->Setinventory();
	if (result < 1)
	{
		invenWG->ClearOverlay();
		SelectedSlot->RemoveFromParent();
		RemoveFromParent();
	}
	else
	{
		SelectedSlot->UpdateSlot();
		CurrAmount = 1;
		Text_Amount->SetText(FText::AsNumber(CurrAmount));
	}
}

