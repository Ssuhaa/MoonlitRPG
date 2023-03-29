// Fill out your copyright notice in the Description page of Project Settings.


#include "NeedItemSelectWG.h"

#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include <UMG/Public/Components/ScrollBox.h>
#include "NeedItemSlotWG.h"
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/WrapBox.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/CanvasPanel.h>


UNeedItemSelectWG::UNeedItemSelectWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UNeedItemSlotWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_NeedItemSlot.WG_NeedItemSlot_C'"));
	if (tempWG.Succeeded())
	{
		WGFactory = tempWG.Class;
	}

	for (int32 i = 0; i < 50; i++)
	{
		UNeedItemSlotWG* currWG = CreateWidget<UNeedItemSlotWG>(GetWorld(), WGFactory);
		NeeditemSlots.Add(currWG);
	}

}

void UNeedItemSelectWG::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetNeedItemSelectWG();
}

// 
// FReply UNeedItemSelectWG::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
// {
// 	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
// 
// 
// 	Removewidget();
// 
// 	return FReply::Handled();
// 
// }

void UNeedItemSelectWG::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	Removewidget();
}

void UNeedItemSelectWG::Removewidget()
{
	RemoveFromParent();
}

void UNeedItemSelectWG::SetNeedItemSelectWG()
{
	player = Cast <ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	
	if (player != nullptr)
	{
		TArray <FInvenItem> WeaponArray = player->InvenComp->FindAllItemsType(EItemType::Outfit);
		for (int32 i = 0; i < WeaponArray.Num(); i++)
		{
			if (WeaponArray[i].InvenID == inventoryData.invenitem.InvenID)
			{
				WeaponArray.RemoveAt(i);
				break;
			}
		}
		

		Wrap_HadWeapon->ClearChildren();
		if (!WeaponArray.IsEmpty())
		{
			Text_Empty->SetVisibility(ESlateVisibility::Hidden);
			for (int32 i = 0; i < WeaponArray.Num(); i++)
			{

				if (!NeeditemSlots.IsValidIndex(i))
				{
					UNeedItemSlotWG* currWG = CreateWidget<UNeedItemSlotWG>(GetWorld(), WGFactory);
					NeeditemSlots.Add(currWG);
				}
				NeeditemSlots[i]->UpdateSlot(WeaponArray[i]);
				NeeditemSlots[i]->NeedSelectWG = this;
				NeeditemSlots[i]->LevelUpSlots = &LevelupSlots;
				Wrap_HadWeapon->AddChild(NeeditemSlots[i]);
			}

		}
		else
		{
			Text_Empty->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UNeedItemSelectWG::UsedItemSlotClear()
{
	for (int32 i = 0; i < NeeditemSlots.Num(); i++)
	{
		if (NeeditemSlots[i]->isSelect)
		{
			NeeditemSlots[i]->RemoveSlot();
		}
	}
}
