// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetSlotBase.h"
#include "QuestSlotWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UQuestSlotWG : public UWidgetSlotBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_Amont;

public:
	virtual void UpdateSlot(FInvenItem invenitem) override;

};
