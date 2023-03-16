// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotWG.h"
#include "LevelUpSlotWG.generated.h"


/**
 * 
 */



UCLASS()
class MOONLITRPG_API ULevelUpSlotWG : public UInventorySlotWG
{
	GENERATED_BODY()

protected:
	virtual void SlotClicked() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_WeaponLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel*Panal_Level;

	UPROPERTY()
	class UTexture2D* resetImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_plus.T_plus'"));
	

public:

	UPROPERTY()
	class UNeedItemSelectWG* NeedSelectWG;

	UPROPERTY()
	class UWeaponLevelUpWG* WeaponLevelUpWG;

	virtual void UpdateSlot(FInvenItem invenData) override;
	void ResetSlot();

	bool isFill = false;
	

};
