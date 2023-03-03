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
	class UWrapBox* WrapBox;
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
	class UButton* Button_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrSlot_Text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Money_Text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* Overlay_Use;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Count;

	UFUNCTION()
	void Setinventory();
	int32 CurrMoney = 0;

	TSubclassOf<class UInventorySlotWG> SlotFactory;
	TSubclassOf<class UinventoryUseButton> ButtonFactory;
	class UinventoryUseButton* ButtonWG;
	TSubclassOf<class UItemDescriptionWG> DescriptionFactory;
	class UItemDescriptionWG* Description;

	EItemType currinventype = EItemType::Consum;

	UFUNCTION()
	void ClickedConsum();
	UFUNCTION()
	void ClickedOutfit();
	UFUNCTION()
	void ClickedFood();
	UFUNCTION()
	void ClickedQuest();
	UFUNCTION()
	void LeftMenu();
	UFUNCTION()
	void RightMenu();

	FInvenItem SelectedSlot;



public:

	class UInventoryComponent* InvenComp;

	UFUNCTION()
	void AddWidget();
	UFUNCTION()
	void RemoveWidget();

	UFUNCTION()
	void ClickedUseButton();

	UFUNCTION()
	void ItemSlotClicked(FInvenItem currSelectItem);

 };