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
	
	UPROPERTY()
	class ASH_Player* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ItemScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UWrapBox* WrapBox;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* itemDescription;

	int32 CurrMoney = 0;

	TSubclassOf<class UInventorySlotWG> SlotFactory;
/*	TSubclassOf<class UinventoryUseButton> ButtonFactory;*/
	TSubclassOf<class UItemDescriptionWG> DescriptionFactory;
	TSubclassOf<class UFoodPopup> FoodPopFactory;
	TArray<TSubclassOf<class UinventoryUseButton>> ButtonFactory;
	

	UPROPERTY()
	class UFoodPopup* FoodPopup;
	UPROPERTY()
	class UinventoryUseButton* ButtonWG;
	UPROPERTY()
	class UinventoryUseButton* OutfitButtonWG;
	UPROPERTY()
	class UItemDescriptionWG* Description;


	EItemType currinventype = EItemType::Count;

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
	UFUNCTION()
	void ClickedUseButton();
	UFUNCTION()
	void ClickedOutfitButton();

	
	void ButtonBinding();

	void ChangeInven(EItemType ChangeInvenType);



	


public:
	UPROPERTY()
	TArray<class UInventorySlotWG*> Slots;
	UPROPERTY()
	class UInventoryComponent* InvenComp;
	int32 SelectedSlotindex;

	UFUNCTION()
	void Setinventory();
	UFUNCTION()
	void AddWidget();
	UFUNCTION()
	void RemoveWidget();
	UFUNCTION()
	void ItemSlotClicked(int32 slotindex);

	void ClearOverlay();



 };