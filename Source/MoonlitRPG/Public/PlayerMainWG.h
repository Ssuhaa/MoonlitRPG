// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataManager.h"
#include "PlayerMainWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UPlayerMainWG : public UUserWidget
{
	GENERATED_BODY()

	UPlayerMainWG(const FObjectInitializer& ObjectInitializer);
	template<typename T>
	T* CreateWGClass(FString path);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* StaminaBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* Overlay_E;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* Overlay_Q;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_ETime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Qpercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_E;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Q;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_CurrHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_TotalHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HPBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* QuestSummaryBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Prograss_E; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Prograss_Q;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* icon_E;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* EReady;	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* QReady;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* QNotReady;

	UPROPERTY()
	class UMaterialInstance* EMat;
	UPROPERTY()
	class UMaterialInstance* QMat;
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicEMat;
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicQMat;

	void VisibleStaminaBar(bool isUseStamina);
	void VisibleSkillText(bool isHand);

	float currentTime = 0;

	UPROPERTY() 
	class ASH_Player* Player;

	UPROPERTY()
	class UQuestSummaryWG* QuestSummary;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* InteractionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* ItemGetBox;

	void UpdateQuestSummary(FQuestInfo CurrQuset);
	void RemoveSummary();
	
	void UpdateStamina(float Stamina, float MaxStamina);
	void UpdateEtime(float Etime, float maxTime);
	void UpdateQPercent(float Qpercent);
	void UpdateHP(float CurrHP, float TotalHP);
};
