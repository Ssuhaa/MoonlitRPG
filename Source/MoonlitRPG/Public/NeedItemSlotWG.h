// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotWG.h"
#include "NeedItemSlotWG.generated.h"


/**
 * 
 */
UCLASS()
class MOONLITRPG_API UNeedItemSlotWG : public UInventorySlotWG
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_WeaponLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Panal_Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* SelectDone;

	virtual void SlotClicked() override;

	UPROPERTY()
	class ULevelUpSlotWG* settingSlot;


public:
	virtual void UpdateSlot(FInvenItem invenData) override;
	void ResetSlot();
	void RemoveSlot();

	bool isSelect = false;


	UPROPERTY()
	class UNeedItemSelectWG* NeedSelectWG;
	TArray<class ULevelUpSlotWG*>* LevelUpSlots;

};
