// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListButtonWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include "QuestComponent.h"

void UQuestListButtonWG::NativeConstruct()
{
	Super::NativeConstruct();

	Button_List->OnPressed.AddUniqueDynamic(this, &UQuestListButtonWG::SendSelectQuest);
}

void UQuestListButtonWG::SetQuestListWG(FQuestInfo* Questinfo)
{	
	SelectQuest = Questinfo;
	TB_QuestName->SetText(FText::FromString(Questinfo->QuestName));
}

void UQuestListButtonWG::SendSelectQuest()
{
	QuestWG->ReciveSelectQuest(SelectQuest);
}

