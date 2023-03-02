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

	UInventorySlotWG(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemAmont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ItemButton;
	UFUNCTION()
	void PopItemDescription();
	TSubclassOf<class UItemDescriptionWG> DescriptionFactory;
	class UItemDescriptionWG* Description;

public:
	class UInventoryWG* invenWG;
	FInvenItem iteminfo;
	void SetItemSlot();

};
