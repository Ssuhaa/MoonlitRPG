// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IH_InteractionUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UIH_InteractionUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* txt_Interaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* OpenAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* PointerAnim;

	bool bAnimPlay = false;

public:
	void PlayOpenAnim();
};
