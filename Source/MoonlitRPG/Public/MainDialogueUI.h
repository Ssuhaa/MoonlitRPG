// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainDialogueUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UMainDialogueUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btn_Close;
	
	UPROPERTY(EditAnywhere)
	class ASH_Player* player;

	UPROPERTY(EditAnywhere)
	class ANPCBase* npc;

	UFUNCTION()
	void CloseButton();
};
