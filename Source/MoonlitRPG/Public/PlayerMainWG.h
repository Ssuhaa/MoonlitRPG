// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UPlayerMainWG : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* StaminaBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_backpack;

	void VisibleStaminaBar(bool isUseStamina);

	float currentTime = 0;

	class ASH_Player* Player;
public:
	
	void UpdateStamina(float Stamina, float MaxStamina);
};
