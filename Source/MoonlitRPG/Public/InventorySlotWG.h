// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "InventorySlotWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UInventorySlotWG : public UUserWidget
{
	GENERATED_BODY()

	UInventorySlotWG (const FObjectInitializer& ObjectInitializer);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ItemButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* SlotBG;
	UFUNCTION()
	void PopItemDescription();

	TArray<class UTexture2D*> BGarray;

public:

	void ButtonBinding();

	int32 Slotindex;
	FInvenItem* selectiteminfo;

	UPROPERTY()
	class UInventoryWG* invenWG;

	void SetItemSlot(FInvenItem* iteminfo);

	void UpdateSlot();

};
