// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataManager.h"
#include "WidgetSlotBase.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UWidgetSlotBase : public UUserWidget
{
	GENERATED_BODY()
protected:
	UWidgetSlotBase(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* SlotBG;


	virtual void ButtonBinding();

	UPROPERTY()
	class ASH_Player* Player; 
	UPROPERTY()
	class ADataManager* DataManager;

	UFUNCTION()
	virtual void SlotClicked();
	virtual void UpdateSlot(FInvenItem* invenitem);

public:
	FinvenData invenData;

};
