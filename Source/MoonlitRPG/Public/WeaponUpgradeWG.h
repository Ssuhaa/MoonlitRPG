// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataManager.h"
#include "WeaponUpgradeWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UWeaponUpgradeWG : public UUserWidget
{
	GENERATED_BODY()
	UWeaponUpgradeWG(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_CurrLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_CurrLevel1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_BeforeMaxLevel;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_AfterMaxLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_CurrPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_UpgradePower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_UpGradeMoney;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* NeedItemList;

	TArray<TSubclassOf<class UUserWidget>> WGFactory;
	UPROPERTY()
	TArray<class UUpgradeSlotWG*> UpgradeSlots;

	int32* PlayerMoney;

	UPROPERTY()
	class ASH_Player* player;

	UPROPERTY()
	class ADataManager* DataManager;

	FinvenData* inventoryData;

	UFUNCTION()
	void UpGrade();

	bool isHaveAllNeedItem();
	void SendToUsedItem();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Upgrade;

	//UFUNCTION()
	void SetUpGradeWG(FinvenData* InvenData);
	
	void ButtonBinding();

	UPROPERTY()
	class UOutfitWG* OutfitWG;

};
