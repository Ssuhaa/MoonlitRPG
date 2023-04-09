// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItemBase.h"
#include "SH_Player.h"
#include "InventoryComponent.h"
#include "PlayerMainWG.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/VerticalBox.h>
#include "IH_InteractionUI.h"


void AWeaponItemBase::GetItem()
{
	if (Player != nullptr)
	{
		Player->InvenComp->WeaponAddItemToinven(iteminfoIndex, WeaponinfoIndex);
		UGameplayStatics::PlaySound2D(GetWorld(), getSound);

		int32 widgetCount = Player->MainHUD->ItemGetBox->GetChildrenCount();
		if (widgetCount < 7)
		{
			Player->MainHUD->ItemGetBox->AddChild(getItemUI);
		}
		
		if (interactionUI != nullptr)
		{
			interactionUI->RemoveFromParent();
		}

		Destroy();
	}
}
