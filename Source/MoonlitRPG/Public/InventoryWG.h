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

DECLARE_MULTICAST_DELEGATE_OneParam(FSandSlot, class UInventorySlotWG*);

UCLASS()
class MOONLITRPG_API UInventoryWG : public UUserWidget
{
	GENERATED_BODY()
	UInventoryWG (const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
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


	TArray<TSubclassOf<class UUserWidget>> WGFactory;
	UPROPERTY()
	class UFoodPopup* FoodPopup;
	UPROPERTY()
	class UinventoryUseButton* ButtonWG;
	UPROPERTY()
	class UItemDescriptionWG* Description;
	UPROPERTY()
	class UOutfitWG* OutfitWG;


	void ButtonBinding();

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
	void ClickedCloseButton();

	

	void ChangeInven(EItemType ChangeInvenType);

	int32 CurrMoney = 0;
	EItemType currinventype = EItemType::Count; //인벤토리에서 버튼들을 클릭했을때 사용.
	EItemType SelectItemType; //인벤토리에서 아이템을 클릭했을 때 사용.



	UPROPERTY()
	TArray<class UInventorySlotWG*> Slots;

public:
	void Setinventory();
	void ClearInvenWGChild();
	void RemoveInventory();

	UFUNCTION()
	void ItemSlotClicked(int32 slotindex);

	UPROPERTY()
	class UInventoryComponent* InvenComp;

	FSandSlot SetSelectSlot;

 };