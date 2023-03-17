// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetSlotBase.h"
#include "NeedItemSlotWG.generated.h"


/**
 * 
 */
UCLASS()
class MOONLITRPG_API UNeedItemSlotWG : public UWidgetSlotBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_WeaponLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* SelectDone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ItemButton;


	virtual void SlotClicked() override;
	virtual void ButtonBinding()override;

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
