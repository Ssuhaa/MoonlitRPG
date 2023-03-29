// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItemBase.h"
#include "SH_Player.h"
#include "InventoryComponent.h"


void AWeaponItemBase::GetItem()
{
	if (Player != nullptr)
	{
		Player->InvenComp->WeaponAddItemToinven(iteminfoIndex, WeaponinfoIndex);
		Destroy();
	}
}
