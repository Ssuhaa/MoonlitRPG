// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestNaviWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UQuestNaviWG : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_Distance;

	void SetDistance();

public:
	
	UPROPERTY()
	class AQuestNaviActor* NaviActor;
};
