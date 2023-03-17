// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetSlotBase.h"
#include "InventorySlotWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UInventorySlotWG : public UWidgetSlotBase
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ItemButton;

	virtual void SlotClicked() override;
	virtual void ButtonBinding() override;

public:

	virtual void UpdateSlot(FInvenItem invenData) override;

};
