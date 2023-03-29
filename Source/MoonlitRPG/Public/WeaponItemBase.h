// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "DataManager.h"
#include "WeaponItemBase.generated.h"

/**
 * 
 */


UCLASS()
class MOONLITRPG_API AWeaponItemBase : public AItemBase
{
	GENERATED_BODY()

public:

	virtual void GetItem() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataIndex)
	int32 WeaponinfoIndex;
};
