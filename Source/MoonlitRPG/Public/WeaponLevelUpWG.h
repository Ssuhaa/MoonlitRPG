// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "WeaponLevelUpWG.generated.h"


/**
 * 
 */
UCLASS()
class MOONLITRPG_API UWeaponLevelUpWG : public UUserWidget
{
	GENERATED_BODY()
	UWeaponLevelUpWG(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_CurrLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_currEXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_MaxEXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_CurrPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_UpgradePower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_levelUpMoney;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_LevelUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Progress_EXP;

	TArray<TSubclassOf<class UUserWidget>> WGFactory;
	UPROPERTY() 
	class UNeedItemSelectWG* NeedSelectWG;
	UPROPERTY()
	TArray<class ULevelUpSlotWG*> LevelUpSlots;



	void ButtonBinding();

	UFUNCTION()
	void LevelUp();

	int32 CurrUseMoney = 0;
	int32 SetCount = 0;
	int32 ToTalEXP = 0;

	TArray<FInvenItem> UseItems;


	UPROPERTY()
	class ASH_Player* Player;


	FInvenItem SelectedSlotItem;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* SelectListBox;

	UFUNCTION()
	void ReceiveSelectSlotData(FInvenItem SelectSlotItem);

	void UpdateUseMoney();

	UFUNCTION()
	void UpdateLevelUpWG();

	UFUNCTION()
	void SendUseditem();

	UPROPERTY()
	class UOutfitWG* OutfitWG;

};
