// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IH_WarpActiveUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UIH_WarpActiveUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ActiveAnim;
};
