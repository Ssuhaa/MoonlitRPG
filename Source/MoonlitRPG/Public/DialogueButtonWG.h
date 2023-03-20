// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueButtonWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UDialogueButtonWG : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_SelectLog;
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Click;
	void SetText(FString Text);

	int32 NextIndex;


	UFUNCTION()
	void ClickedButton();

	UPROPERTY()
	class UMainDialogueUI* DialogueWG;
	
};
