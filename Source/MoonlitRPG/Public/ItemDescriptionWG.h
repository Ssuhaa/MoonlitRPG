// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "ItemDescriptionWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UItemDescriptionWG : public UUserWidget
{
	GENERATED_BODY()

	UItemDescriptionWG(const FObjectInitializer& ObjectInitializer);
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* BG;

	TArray<class UTexture2D*> BGarray;

public:

	void SetDescription(class UInventorySlotWG* SelectedSlot);

};
