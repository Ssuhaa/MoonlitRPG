// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataManager.h"
#include "OutfitWG.generated.h"

/**
 * 
 */

 DECLARE_MULTICAST_DELEGATE_OneParam(FSendInvenData, FinvenData*);

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
	template<typename T>
	T* CreateWGClass(FString path);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* itemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Upgrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_LevelUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Change;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Off;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Money;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_Wearing;

	UFUNCTION()
	void OnclickedDetail();	
	UFUNCTION()
	void OnclickedUpgrade();
	UFUNCTION()
	void OnclickedLevelUp();
	UFUNCTION()
	void OnclickedWearing();
	UFUNCTION()
	void OnclickedOff();
	UFUNCTION()
	void OnclickedBack();

	void WearingSwitch();
	void ButtonSwitch(bool isMaxLevel);
	void ReinforceSwitch(EEquipmentState state);

	UPROPERTY()
	class UItemDescriptionWG* description;
	UPROPERTY()
	class UWeaponUpgradeWG* UpgradeWG;
	UPROPERTY()
	class UWeaponLevelUpWG* LevelUpWG;

	UPROPERTY()
	class ASH_Player* player;

	UPROPERTY()
	class APreviewActor* OutfitActor;


	FinvenData* inventoryData;

	void ButtonBinding();

	FSendInvenData SendToInvenInfo;

	void SetOutfItWG();


public:
	UFUNCTION()
	void UpdateOutfitWG();
	void ReceiveSelectSlotData(FinvenData* invenData);
	void ReceiveUseItem(FInvenItem* ModifiedItem);
};

