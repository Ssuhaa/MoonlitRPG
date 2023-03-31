// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenShotUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UScreenShotUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btn_ScreenShot;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* ScreenshotAnim;

	UFUNCTION()
	void TakeScreenShot();

	void ScreenShot(FString FileName);
};
