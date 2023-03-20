// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueButtonWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>

void UDialogueButtonWG::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Click->OnPressed.AddUniqueDynamic(this, &UDialogueButtonWG::ClickedButton);
}

void UDialogueButtonWG::SetText(FString Text)
{
	Text_SelectLog->SetText(FText::FromString(Text));
}

void UDialogueButtonWG::ClickedButton()
{
	DialogueWG->OnClikedNextButton(NextIndex);
}
