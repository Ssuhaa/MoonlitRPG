// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
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

	TArray<class UTexture2D*> BGarray;

	virtual void ButtonBinding();

	UPROPERTY()
	class ASH_Player* player; 

public:
	UFUNCTION()
	virtual void SlotClicked();
	virtual void UpdateSlot(FInvenItem invenData);

	UPROPERTY()
	class UInventoryWG* invenWG;
	int32 Slotindex;
	FInvenItem invenInfo;


};
