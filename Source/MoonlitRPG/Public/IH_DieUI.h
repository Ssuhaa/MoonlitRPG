// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IH_DieUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UIH_DieUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* btn_Revive;

	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* PlayerDie;

	UPROPERTY(EditAnywhere)
	class ASH_Player* player;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> warpPointArray;

	UPROPERTY(EditAnywhere)
	float minDist = 10000;

public:
	UFUNCTION()
	void ReviveButton();

	void CallRevive();
	void LoadingRemove();
	void FindWarpPoint();
};