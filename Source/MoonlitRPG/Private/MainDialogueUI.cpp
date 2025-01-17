﻿// Fill out your copyright notice in the Description page of Project Settings.


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
#include <UMG/Public/Components/CanvasPanelSlot.h>

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

	player->MainHUD->SetVisibility(ESlateVisibility::Hidden);
	player->DisableInput(player->playerCon);
	player->playerCon->bShowMouseCursor = true;
	
	PlayAnimation(DialogueOpenAnim);
}

void UMainDialogueUI::NativeDestruct()
{
	Super::NativeDestruct();
	if (player->QuestComp->isDoneQuestRequirements(player->QuestComp->MainQuest)) //만약 퀘스트 수행이 완료되었다면
	{
		player->QuestComp->CompleteMainQuest();
		return;
	}
}

void UMainDialogueUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (isTextPlay == true)
	{
		currTime += InDeltaTime;
		if (TextLangth <= CsvColumns[1].Len() && currTime > showTime)
		{
			currTime = 0;

			Dialogue->SetText(FText::FromString(CsvColumns[1].Mid(0, TextLangth)));
			TextLangth ++;
			if (TextLangth > CsvColumns[1].Len())
			{
				TextLangth = 0;
				isTextPlay = false;
				VB_Choices->SetVisibility(ESlateVisibility::Visible);
				if (CsvColumns[2] == TEXT("None"))
				{
					Pointer->SetVisibility(ESlateVisibility::Visible);
				}

			}
		}
	}
}

FReply UMainDialogueUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
	if(!CsvColumns.IsValidIndex(2)) return FReply::Unhandled();
	if (Pointer->IsVisible() && isTextPlay == false)
	{
		isTextPlay = true;
		UGameplayStatics::PlaySound2D(GetWorld(), player->SoundArr[3]);
		CurrNext ++;
		SetDialogue(CurrNext);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}


void UMainDialogueUI::ClosedDialouge()
{
	if (npc != nullptr)
	{
		player->bTalking = false;
		//npc->bTalking = false;
		//npc = nullptr;
	}
	player->playerCon->SetViewTargetWithBlend(player, 0.5f, VTBlend_EaseInOut, 1.0f);
	player->playerCon->bShowMouseCursor = false;
	player->EnableInput(player->playerCon);
	player->MainHUD->SetVisibility(ESlateVisibility::Visible);
	RemoveFromParent();
}

void UMainDialogueUI::SetStartDialouge(int32 Start)
{
	CurrNext = Start;
}

void UMainDialogueUI::ReadCSVFile(FString CSVPath)
{
	if (FFileHelper::LoadFileToStringArray(CsvRows, *CSVPath))
	{
		SetDialogue(CurrNext);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file: %s"), *CSVPath);
	}
}



void UMainDialogueUI::MainQuestContect(ANPCBase* ContectNPC)
{
	npc = ContectNPC;
	SetStartDialouge(player->QuestComp->MainQuest->DialougueIndex);
	SetDialogue(CurrNext);
	ReadCSVFile(FPaths::ProjectContentDir() + FString::Printf(TEXT("Dialogue/MainQuest.csv")));
	AddToViewport();
}

void UMainDialogueUI::CommonContect(ANPCBase* ContectNPC)
{
	npc = ContectNPC;
	SetStartDialouge(1);
	SetDialogue(CurrNext);
	ReadCSVFile(ContectNPC->MakeCommonCSVPath());
	AddToViewport();
}

void UMainDialogueUI::SetDialogue(int32 Next)
{
	if (CsvRows.IsEmpty())
	{
		return;
	}
	else if (CsvRows.IsValidIndex(Next))
	{
		CsvRows[Next].ParseIntoArray(CsvColumns, TEXT(",")); //콤마를 기준으로 단어를 끊어서 CSV열에 담아라.
	}
	else
	{
		ClosedDialouge();
		return;
	}

	if (CsvColumns[0] == TEXT("Cut"))
	{
		ClosedDialouge();
		return;
	}

	//이름[0], 내용[1], 선택지1[2], Next1[3], 선택지2[4], Next2[5], 선택지3[6], Next3[7]
	for (int32 i = 1; i < CsvColumns.Num(); i++)
	{
		CsvColumns[i].ReplaceInline(TEXT("{PlayerName}"), *PlayerName);
		CsvColumns[i].ReplaceInline(TEXT("\\n"), TEXT("\n"));

	}

	Name->SetText(FText::FromString(CsvColumns[0]));
	
	isTextPlay = true;
	
	VB_Choices->SetVisibility(ESlateVisibility::Hidden);
	VB_Choices->ClearChildren();
	UPanelSlot* pSlot = VB_Choices->Slot;
	UCanvasPanelSlot* canvas = Cast<UCanvasPanelSlot>(pSlot);

	if(CsvColumns[2] != TEXT("None"))
	{

		Buttons[0]->SetText(CsvColumns[2]);
		Buttons[0]->DialogueWG = this;
	
		canvas->SetPosition(FVector2D(236.0, 64.0));
		VB_Choices->AddChild(Buttons[0]);
	
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
		Buttons[1]->SetText(CsvColumns[4]);
		Buttons[1]->DialogueWG = this;
		
		VB_Choices->AddChildToVerticalBox(Buttons[1]);
		canvas->SetPosition(FVector2D(236.0, 4.0));

		Buttons[1]->NextIndex = FCString::Atoi(*CsvColumns[5]  );
	}
	else return;


	if (CsvColumns[6] != TEXT("None"))
	{

		Buttons[2]->SetText(CsvColumns[6]);
		Buttons[2]->DialogueWG = this;
		VB_Choices->AddChildToVerticalBox(Buttons[2]);
		canvas->SetPosition(FVector2D(236.0, -64.0));

		Buttons[2]->NextIndex = FCString::Atoi(*CsvColumns[7]);
	}
	
	
}

void UMainDialogueUI::OnClikedNextButton(int32 Nextindex)
{
	CurrNext = Nextindex;
	SetDialogue(CurrNext); //ROW, 행을 지정해서 대화 세팅.
}
