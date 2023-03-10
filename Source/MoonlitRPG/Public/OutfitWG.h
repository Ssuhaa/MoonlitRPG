// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

protected:
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


public:
	void ButtonBinding();
	void PopupOutfit(class UItemDescriptionWG* Description);
};
