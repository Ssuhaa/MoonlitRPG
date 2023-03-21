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
	UMainDialogueUI(const FObjectInitializer& ObjectInitializer);
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btn_Close;
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
	void CloseButton();

	TSubclassOf<class UDialogueButtonWG> buttonFactory;

	UPROPERTY()
	TArray<class UDialogueButtonWG*> Buttons;

	int32 CurrNext = 0;
	void SetDialogue(int32 Next);



public:

	bool ReadCSVFile(FString CVSName);
	UFUNCTION()
	void OnClikedNextButton(int32 Nextindex);

	TArray<FString> CsvRows;
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
