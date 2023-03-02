// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "ItemBase.h"
#include "InventoryWG.generated.h"


/**
 * 
 */
UCLASS()
class MOONLITRPG_API UInventoryWG : public UUserWidget
{
	GENERATED_BODY()
	UInventoryWG (const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	class ASH_Player* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ItemScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* Overlay_ItemInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Close;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Consum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Outfit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Food;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Quest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UWrapBox* WrapBox;


	TArray<FInvenItem> invenArray;
	void Setinventory();

	TSubclassOf<class UInventorySlotWG> SlotFactory;

	EItemType currinventype = EItemType::Consum;

	UFUNCTION()
	void ClickedConsum();
	UFUNCTION()
	void ClickedOutfit();
	UFUNCTION()
	void ClickedFood();
	UFUNCTION()
	void ClickedQuest();

public:
	void AddWidget(TArray<FInvenItem> invenArr);
	UFUNCTION()
	void RemoveWidget();

	void ItemSlotClicked(class UItemDescriptionWG* descriptionWG);
 };