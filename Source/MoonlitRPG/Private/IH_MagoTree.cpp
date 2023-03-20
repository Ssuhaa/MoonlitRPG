// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_MagoTree.h"
#include "MainDialogueUI.h"
#include <UMG/Public/Components/TextBlock.h>

AIH_MagoTree::AIH_MagoTree()
{
	ConstructorHelpers::FClassFinder<UMainDialogueUI>tempdialogueUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Dialogue.WG_Dialogue_C'"));
	if (tempdialogueUI.Succeeded())
	{
		dialogueUIFactory = tempdialogueUI.Class;
	}
}

void AIH_MagoTree::BeginPlay()
{
	Super::BeginPlay();

	dialogueUI = CreateWidget<UMainDialogueUI>(GetWorld(), dialogueUIFactory);
}

void AIH_MagoTree::Interaction()
{
	dialogueUI->AddToViewport();
	dialogueUI->Name->SetText(FText(InteractName));
}