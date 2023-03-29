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
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ItemButton;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* AddSlot;

	virtual void SlotClicked() override;
	virtual void ButtonBinding() override;

public:

	virtual void UpdateSlot(FInvenItem invenitem) override;
	UPROPERTY()
	class UInventoryWG* InvenWG;
};
