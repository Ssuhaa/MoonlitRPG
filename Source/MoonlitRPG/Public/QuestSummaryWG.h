// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSummaryWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UQuestSummaryWG : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* ChangeSummary;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Summary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* QusetName;



	
};
