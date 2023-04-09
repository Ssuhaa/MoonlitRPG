// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UStartMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient);
// 	class UWidgetAnimation* StartHover;
 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient);
// 	class UWidgetAnimation* QuitHover;

	bool gameStart = false;
	float currTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_Quit;

	UFUNCTION()
	void ClickStart();
	UFUNCTION()
	void ClickQuit();
};