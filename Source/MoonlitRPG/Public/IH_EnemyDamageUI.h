// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IH_EnemyDamageUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UIH_EnemyDamageUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FloatAnimation;
};
