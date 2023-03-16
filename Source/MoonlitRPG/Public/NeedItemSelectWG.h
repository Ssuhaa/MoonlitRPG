// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "NeedItemSelectWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UNeedItemSelectWG : public UUserWidget
{
	GENERATED_BODY()

	UNeedItemSelectWG(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Close;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Empty;

	TSubclassOf<class UUserWidget> WGFactory;
	UPROPERTY()
	TArray<class UNeedItemSlotWG*> NeeditemSlots;


	UPROPERTY()
	class ASH_Player* player;



	UFUNCTION()
	void Removewidget();

	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UWrapBox* Wrap_HadWeapon;

	
	FInvenItem SelectedSlotItem;

	TArray<class ULevelUpSlotWG*> LevelupSlots;


	void SelectNeedItem(FInvenItem* invenData);


	void UsedItemSlotClear();

	UFUNCTION()
	void SetNeedItemSelectWG();
};
