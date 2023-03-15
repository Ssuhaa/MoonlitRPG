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

	virtual void SlotClicked() override;

	bool isSelect = false;
	int32 Selectindex;

	UPROPERTY()
	class ULevelUpSlotWG* pSelect;


public:
	virtual void UpdateSlot(FInvenItem* invenData) override;

	UPROPERTY()
	class UNeedItemSelectWG* NeedSelectWG;


	TArray<class ULevelUpSlotWG*>* LevelUpSlots;

};
