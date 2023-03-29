// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetSlotBase.h"
#include "UpgradeSlotWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UUpgradeSlotWG : public UWidgetSlotBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_NeedAmount;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_CurrAmount;

public:
	virtual void UpdateSlot(FInvenItem invenitem) override;

	int32 Index;
	bool isHaveAmount = false;
	FInvenItem UpgradeItem;
	int32 UseAmount;
};
