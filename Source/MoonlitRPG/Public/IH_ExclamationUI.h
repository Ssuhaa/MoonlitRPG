
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IH_ExclamationUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UIH_ExclamationUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* LookAnimation;
};
