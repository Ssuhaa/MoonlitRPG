// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataManager.h"
#include "QuestListButtonWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UQuestListButtonWG : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_List;

	

	UFUNCTION()
	void SendSelectQuest();

public:
	void SetQuestListWG(FQuestInfo* Questinfo);

	UPROPERTY()
	class UQuestWG* QuestWG;
	
	FQuestInfo* SelectQuest;
};
