// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "OutfitWG.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEquipmentState : uint8
{
	Detail,
	Upgrade,
	LevelUp,
};

UCLASS()
class MOONLITRPG_API UOutfitWG : public UUserWidget
{
	GENERATED_BODY()

	UOutfitWG(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* itemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Upgrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_LevelUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Wearing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Back;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Panel_Detail;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Panel_Reinforce;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
// 	class UCanvasPanel* Panel_LevelUp;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
// 	class UCanvasPanel* Panel_Upgrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* EquipPop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Money;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
// 	class UWeaponUpgradeWG* WG_WeaponUpgrade;


	EEquipmentState OutfitState = EEquipmentState::Detail;

	UFUNCTION()
	void OnclickedDetail();	
	UFUNCTION()
	void OnclickedUpgrade();
	UFUNCTION()
	void OnclickedLevelUp();
	UFUNCTION()
	void OnclickedWearing();
	UFUNCTION()
	void OnclickedBack();

	void WearingSwitch(bool isEquip);
	void ButtonSwitch(bool isMaxLevel);
	void ReinforceSwitch(EEquipmentState state);

	TArray<TSubclassOf<class UUserWidget>> WGFactory; 
	UPROPERTY()
	class UItemDescriptionWG* description;
	UPROPERTY()
	class UWeaponUpgradeWG* UpgradeWG;

	UPROPERTY()
	class ASH_Player* player;
	
	FInvenItem* SelectInveninfo;


	UFUNCTION()
	void UpdateMoney();

public:
	void ButtonBinding();
	void SetOutfitWG(class UInventorySlotWG* SelectSlot);
};
