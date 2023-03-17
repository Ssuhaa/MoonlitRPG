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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Panel_Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Panel_Food;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Heal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border_Equip;



	TArray<class UTexture2D*> BGarray;


	FInvenItem SelectedSlotItem;



	UPROPERTY()
	class UInventorySlotWG* SelectedSlot;

public:

	UFUNCTION()
	void ReceiveSelectSlotitemData(FInvenItem SelectSlotItem);

	UFUNCTION()
	void ReceiveSelectSlotData(class UInventorySlotWG* SelectSlot);

	void UpdateDescription();

};
