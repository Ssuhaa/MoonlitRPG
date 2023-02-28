// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IH_EnemyHPUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UIH_EnemyHPUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* enemyHP_Bar;

public:
	void UpdateHP(float currHP, float maxHP);
};
