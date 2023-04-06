// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestComponent.h"
#include "QuestWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UQuestWG : public UUserWidget
{
	GENERATED_BODY()

	UQuestWG(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_All;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Main;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Sub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Today;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Close;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* VB_QuestList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* SB_Description;

	void ButtonBinding();
	UFUNCTION()
	void ClickedAllButton();
	UFUNCTION()
	void ClickedMainButton();
	UFUNCTION()
	void ClickedSubButton();
	UFUNCTION()
	void ClickedTodayButton();
	UFUNCTION()
	void ClickedCloseButton();


	TArray<TSubclassOf<class UUserWidget>> WGFactory;
	UPROPERTY()
	TArray<class UQuestListButtonWG*> QuestButtons;
	UPROPERTY()
	class UQuestDescriptionWG* QuestDescription;


	UPROPERTY()
	class ASH_Player* Player;
	UPROPERTY()
	class ADataManager* DataManager;
	
	void ChangeQuestList(EQuestType ChangeType);
	void SetQuestList(TArray<TObjectPtr<FQuestInfo>> QuestList);

	EQuestType CurrQuestType = EQuestType::Total;

	

public:

	void UpdateQuestList();
	void RemoveQuestWG();


	void ReciveSelectQuest(TObjectPtr<FQuestInfo> SelectQuest);
	
};
