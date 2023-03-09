// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "FoodPopup.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UFoodPopup : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Use;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Close;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Minus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* PlayerImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealHPBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* CurrHPBar;

	UPROPERTY()
	class ASH_Player* Player;

	
	UFUNCTION()
	void OnClickedCancel();
	UFUNCTION()
	void OnClickedUse();
	UFUNCTION()
	void OnClickedClose();
	UFUNCTION()
	void OnClickedPlus();
	UFUNCTION()
	void OnClickedMinus();

	int32 CurrAmount = 1;

	float HP;
	float HealHP;

	void UpdateHPBar();

	int32 selectindex;
	
	FInvenItem iteminfo;

public:

	UPROPERTY()
	class UInventoryWG* invenWG;

	void SetFoodPop(FInvenItem selectitem);
	void ButtonBinding();
};
