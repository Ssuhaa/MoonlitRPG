// Fill out your copyright notice in the Description page of Project Settings.


#include "NeedItemSelectWG.h"

#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include <UMG/Public/Components/ScrollBox.h>
#include "NeedItemSlotWG.h"
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/WrapBox.h>
#include <UMG/Public/Components/TextBlock.h>

UNeedItemSelectWG::UNeedItemSelectWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UNeedItemSlotWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_NeedItemSlot.WG_NeedItemSlot_C'"));
	if (tempWG.Succeeded())
	{
		WGFactory = tempWG.Class;
	}

	for (int32 i = 0; i < 100; i++)
	{
		UNeedItemSlotWG* currWG = CreateWidget<UNeedItemSlotWG>(GetWorld(), WGFactory);
		NeeditemSlots.Add(currWG);
	}

}

void UNeedItemSelectWG::NativeConstruct()
{
	Super::NativeConstruct();

	SetNeedItemSelectWG();

	Button_Close->OnPressed.AddUniqueDynamic(this, &UNeedItemSelectWG::Removewidget); // 다른곳 누르면 사라지게하는.. 수정필요

}

void UNeedItemSelectWG::Removewidget()
{
	RemoveFromParent();
}

void UNeedItemSelectWG::SetNeedItemSelectWG()
{
	WeaponArray.Empty();
	player = Cast <ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (player != nullptr)
	{
		for (int32 i = 0; i < player->InvenComp->invenItemArr.Num(); i++)
		{
			if (&player->InvenComp->invenItemArr[i] == SelectedSlot->invenInfo) continue;
				if (player->InvenComp->invenItemArr[i].iteminfomation.itemType == EItemType::Outfit)
				{

					WeaponArray.AddUnique(&player->InvenComp->invenItemArr[i]);

				}
			
			
		}
	}
	SetSlot();	
}

void UNeedItemSelectWG::SetSlot()
{
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
			NeeditemSlots[i]->LevelUpSlots = &LevelupSlots;
			Wrap_HadWeapon->AddChild(NeeditemSlots[i]);
		}

	}
	else
	{
		Text_Empty->SetVisibility(ESlateVisibility::Visible);
	}
}
