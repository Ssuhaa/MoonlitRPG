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
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* txt_Interaction;
};
