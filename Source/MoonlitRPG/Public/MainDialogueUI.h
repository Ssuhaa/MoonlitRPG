// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataManager.h"
#include "MainDialogueUI.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UMainDialogueUI : public UUserWidget
{
	GENERATED_BODY()
	UMainDialogueUI(const FObjectInitializer& ObjectInitializer);
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Name;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Dialogue;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Pointer;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* VB_Choices;


	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* PointerAnim;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* DialogueOpenAnim;
	
	UPROPERTY(EditAnywhere)
	class ASH_Player* player;

	UPROPERTY(EditAnywhere)
	class ANPCBase* npc;

	UFUNCTION()
	void ClosedDialouge();

	TSubclassOf<class UDialogueButtonWG> buttonFactory;

	UPROPERTY()
	TArray<class UDialogueButtonWG*> Buttons;

	int32 CurrNext = 1;
	void SetDialogue(int32 Next);

	UPROPERTY()
	class ADataManager* DataManager;

public:

	void SetStartDialouge(int32 Start);

	void ReadCSVFile(FString CSVPath);
	UFUNCTION()
	void OnClikedNextButton(int32 Nextindex);

	UPROPERTY()
	TArray<FString> CsvRows;
	UPROPERTY()
	TArray<FString> CsvColumns;

	FString Text;
	FString Select1;
	FString Select2;
	FString Select3;
	FString CharacterName;
	int32 Next1;
	int32 Next2;
	int32 Next3;

	FString PlayerName = TEXT("익현");
};
