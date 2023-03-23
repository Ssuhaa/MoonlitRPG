// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDescriptionWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UQuestDescriptionWG : public UUserWidget
{
	GENERATED_BODY()
	UQuestDescriptionWG(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_QName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_QLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_QDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_QMoney;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_QEXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_Reward;

	TSubclassOf<class UQuestSlotWG> SlotFactory;
	TArray<class UQuestSlotWG*> RewardSlots;

public:
	void SetQuestDescription(FQuestInfo* QuestInfo);
};
