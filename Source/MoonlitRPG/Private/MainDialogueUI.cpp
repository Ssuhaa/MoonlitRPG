// Fill out your copyright notice in the Description page of Project Settings.


#include "MainDialogueUI.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "NPCBase.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include <UMG/Public/Components/TextBlock.h>
#include "DialogueButtonWG.h"
#include <UMG/Public/Components/ScrollBox.h>
#include <UMG/Public/Components/VerticalBox.h>
#include "QuestComponent.h"
#include "DataManager.h"

UMainDialogueUI::UMainDialogueUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UDialogueButtonWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_DialogueButton.WG_DialogueButton_C'"));
	if (tempWG.Succeeded())
	{
		buttonFactory = tempWG.Class;
	}

	for(int32 i=0; i<3;i++)
	{
		UDialogueButtonWG* CurrWG = CreateWidget<UDialogueButtonWG>(GetWorld(),buttonFactory);
		Buttons.Add(CurrWG);
	}

	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
}

void UMainDialogueUI::NativeConstruct()
{
	Super::NativeConstruct();


	player->DisableInput(player->playerCon);
	player->playerCon->bShowMouseCursor = true;
	
	btn_Close->OnPressed.AddUniqueDynamic(this, &UMainDialogueUI::CloseButton);
	PlayAnimation(DialogueOpenAnim);


}

FReply UMainDialogueUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
	if (CsvColumns[2] == TEXT("None"))
	{
		CurrNext ++;
		SetDialogue(CurrNext);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}


void UMainDialogueUI::CloseButton()
{
	if (npc != nullptr)
	{
		npc->bTalking = false;
		npc = nullptr;
	}

	player->playerCon->bShowMouseCursor = false;
	player->EnableInput(player->playerCon);
	RemoveFromParent();
}

void UMainDialogueUI::ReadCSVFile(FString CSVPath)
{
	if (FFileHelper::LoadFileToStringArray(CsvRows, *CSVPath))
	{
		CurrNext = DataManager->GetInfo(0, DataManager->MainQuestList).DialougueIndex;
		SetDialogue(CurrNext);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file: %s"), *CSVPath);
	}
}


void UMainDialogueUI::SetDialogue(int32 Next)
{
	
	if (CsvRows.IsValidIndex(Next))
	{
		CsvRows[Next].ParseIntoArray(CsvColumns, TEXT(",")); //콤마를 기준으로 단어를 끊어서 CSV열에 담아라.
	}
	else 
	{
		
		player->QuestComp->CompleteMainQuest();
		CloseButton();
		return;
	}



	//이름[0], 내용[1], 선택지1[2], Next1[3], 선택지2[4], Next2[5], 선택지3[6], Next3[7]
	for (int32 i = 1; i < CsvColumns.Num(); i++)
	{
		CsvColumns[i].ReplaceInline(TEXT("{PlayerName}"), *PlayerName);
		CsvColumns[i].ReplaceInline(TEXT("\\n"), TEXT("\n"));

	}

	Name->SetText(FText::FromString(CsvColumns[0]));
	

	Dialogue->SetText(FText::FromString(CsvColumns[1]));

	VB_Choices->ClearChildren();

	if(CsvColumns[2] != TEXT("None"))
	{
	
		//Select1 = CsvColumns[2];
		Buttons[0]->SetText(CsvColumns[2]);
		Buttons[0]->DialogueWG = this;
		VB_Choices->AddChild(Buttons[0]);
	
		//Next1 = FCString::Atoi(*CsvColumns[3]);
		Buttons[0]->NextIndex = FCString::Atoi(*CsvColumns[3]);

		Pointer->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Pointer->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(PointerAnim, 0.0f, 0);
		return;
	}

	if (CsvColumns[4] != TEXT("None"))
	{
		//Select2 = CsvColumns[4];
		Buttons[1]->SetText(CsvColumns[4]);
		Buttons[1]->DialogueWG = this;
		VB_Choices->AddChild(Buttons[1]);

		//Next2 = FCString::Atoi(*CsvColumns[5]);
		Buttons[1]->NextIndex = FCString::Atoi(*CsvColumns[5]);
	}
	else return;


	if (CsvColumns[6] != TEXT("None"))
	{

		//Select3 = CsvColumns[6];
		Buttons[2]->SetText(CsvColumns[6]);
		Buttons[2]->DialogueWG = this;
		VB_Choices->AddChild(Buttons[2]);

		//Next3 = FCString::Atoi(*CsvColumns[7]);
		Buttons[2]->NextIndex = FCString::Atoi(*CsvColumns[7]);
	}
	
}

void UMainDialogueUI::OnClikedNextButton(int32 Nextindex)
{
	CurrNext = Nextindex;
	SetDialogue(CurrNext); //ROW, 행을 지정해서 대화 세팅.
}
