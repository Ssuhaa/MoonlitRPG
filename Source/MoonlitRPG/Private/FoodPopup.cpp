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
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
}

void UFoodPopup::NativeConstruct()
{
	Super::NativeConstruct();
	ButtonBinding();
	CurrAmount = 1;
	Text_Amount->SetText(FText::AsNumber(CurrAmount));
}

void UFoodPopup::ButtonBinding()
{
	Button_Cancel->OnPressed.AddUniqueDynamic(this, &UFoodPopup::OnClickedCancel);
	Button_Use->OnPressed.AddUniqueDynamic(this, &UFoodPopup::OnClickedUse);
	Button_Close->OnPressed.AddUniqueDynamic(this, &UFoodPopup::OnClickedClose);
	Button_Plus->OnPressed.AddUniqueDynamic(this, &UFoodPopup::OnClickedPlus);
	Button_Minus->OnPressed.AddUniqueDynamic(this, &UFoodPopup::OnClickedMinus);
	
	FullPopup->Button_Use->OnPressed.AddUniqueDynamic(this, &UFoodPopup::OnClickedFullPopup);
}

void UFoodPopup::ReceiveSelectSlotData(FinvenData invenData)
{
	inventoryData = invenData;
	ItemImage->SetBrushFromTexture(inventoryData.iteminfo.itemImage);
	UpdateHPBar();
}

void UFoodPopup::UpdateHPBar()
{
	if (Player != nullptr)
	{
		HP = Player->PlayercurrHP / Player->PlayerTotalHP;
		CurrHPBar->SetPercent(HP);
		HealHP = (inventoryData.iteminfo.HealAmount * CurrAmount) / Player->PlayerTotalHP;
		HealHPBar->SetPercent(HP + HealHP);
	}
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
			CurrAmount = 1;
			Text_Amount->SetText(FText::AsNumber(CurrAmount));
			UpdateHPBar();
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
	CurrAmount = FMath::Clamp(CurrAmount, 1, inventoryData.invenitem.itemAmount);
	Text_Amount->SetText(FText::AsNumber(CurrAmount));
	UpdateHPBar();
}

void UFoodPopup::OnClickedMinus()
{
	CurrAmount--;
	CurrAmount = FMath::Clamp(CurrAmount, 1, inventoryData.invenitem.itemAmount);
	Text_Amount->SetText(FText::AsNumber(CurrAmount));
	UpdateHPBar();
}

void UFoodPopup::OnClickedFullPopup()
{
	HealCallAndUpdatePopup();
	FullPopup->RemoveFromParent();
}



void UFoodPopup::HealCallAndUpdatePopup()
{
	Player->HealPlayer(inventoryData.iteminfo.HealAmount * CurrAmount);
	int32 result = Player->InvenComp->MinusItemAmount(inventoryData.invenitem, CurrAmount);
	invenWG->Setinventory();
	if (result < 1)
	{
		RemoveFromParent();
	}
}

