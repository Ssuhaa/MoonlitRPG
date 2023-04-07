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

DECLARE_MULTICAST_DELEGATE_OneParam(FSendSlot, FinvenData*);

UCLASS()
class MOONLITRPG_API UInventoryWG : public UUserWidget
{
	GENERATED_BODY()
	
	UInventoryWG (const FObjectInitializer& ObjectInitializer);

	template<typename T>
	T* CreateWGClass(FString path);
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
	class UButton* Button_ETC;
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
	void ClickedETC();
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
	EItemType currinventype = EItemType::Consum; //인벤토리에서 버튼들을 클릭했을때 사용.



	UPROPERTY()
	TArray<class UInventorySlotWG*> Slots;

	UPROPERTY()
	class ADataManager* DataManager;

	UPROPERTY()
	TArray<class UButton*> Buttons;
	FLinearColor Gray = { 0.040915, 0.040915, 0.040915, 1.000000 };
	FLinearColor White = { 0.838799, 0.838799, 0.838799, 1.000000 };

public:
	void Setinventory();
	void ClearInvenWGChild();
	void RemoveInventory();

	//UFUNCTION()
	void ItemSlotClicked(FinvenData* invenData);

	UPROPERTY()
	class UInventoryComponent* InvenComp;

	FSendSlot SendInvenData;

	void ChangeButtonColor(class UButton* SelectButton);
	void StopSelectAnim();
 };

