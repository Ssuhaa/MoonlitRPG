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
}

void UMainDialogueUI::NativeConstruct()
{
	Super::NativeConstruct();
	ReadCSVFile(TEXT("Dialogue"));

	
	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	player->DisableInput(player->playerCon);
	player->playerCon->bShowMouseCursor = true;
	
	btn_Close->OnPressed.AddUniqueDynamic(this, &UMainDialogueUI::CloseButton);

	PlayAnimation(DialogueOpenAnim);
}

FReply UMainDialogueUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
	if (!VB_Choices->HasChild(Buttons[0]))
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

bool UMainDialogueUI::ReadCSVFile(FString CVSName)
{
	FString CsvFilePath = FPaths::ProjectContentDir() + FString::Printf(TEXT("Dialogue/%s.csv"), *CVSName);
	
	if (!FFileHelper::LoadFileToStringArray(CsvRows, *CsvFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file: %s"), *CsvFilePath);
		return false;
	}
	else
	{
		CurrNext = 0;
		SetDialogue(CurrNext);
	}

	return true;
}


void UMainDialogueUI::SetDialogue(int32 Next)
{
	TArray<FString> CsvColumns;

	if (CsvRows.IsValidIndex(Next))
	{
		CsvRows[Next].ParseIntoArray(CsvColumns, TEXT(",")); //콤마를 기준으로 단어를 끊어서 CSV열에 담아라.
	}

	VB_Choices->ClearChildren();
	if (CsvColumns.Num() < 3)
	{
		Pointer->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(PointerAnim, 0.0f, 0);
	}
	else
	{
		Pointer->SetVisibility(ESlateVisibility::Hidden);
	}

	//이름[0], 내용[1], 선택지1[2], Next1[3], 선택지2[4], Next2[5], 선택지3[6], Next3[7]
	for (int32 i = 0; i < CsvColumns.Num(); i++)
	{
		switch (i)
		{
		case 0: // 이름 세팅
			CharacterName = CsvColumns[0];
			Name->SetText(FText::FromString(CharacterName));
			break;
		case 1: //내용 세팅
			Text = CsvColumns[1];
			Dialogue->SetText(FText::FromString(Text));
			break;
		case 2: //선택지 세팅을 한 후 스크롤에 차일드 시킴.
			Select1 = CsvColumns[2];
			Buttons[0]->SetText(Select1);
			Buttons[0]->DialogueWG = this;
			VB_Choices->AddChild(Buttons[0]);
			break;
		case 3: //스트링을 숫자로 바꿔서 Next에 저장.
			Next1 = FCString::Atoi(*CsvColumns[3]);
			Buttons[0]->NextIndex = Next1;
			break;
		case 4:
			Select2 = CsvColumns[4];
			Buttons[1]->SetText(Select2);
			Buttons[1]->DialogueWG = this;
			VB_Choices->AddChild(Buttons[1]);
			break;
		case 5:
			Next2 = FCString::Atoi(*CsvColumns[5]);
			Buttons[1]->NextIndex = Next2;
			break;
		case 6:
			Select3 = CsvColumns[6];
			Buttons[2]->SetText(Select3);
			Buttons[2]->DialogueWG = this;
			VB_Choices->AddChild(Buttons[3]);
			break;
		case 7:
			Next3 = FCString::Atoi(*CsvColumns[7]);
			Buttons[2]->NextIndex = Next3;
			break;
		}
	}
	
}

void UMainDialogueUI::OnClikedNextButton(int32 Nextindex)
{
	CurrNext = Nextindex;
	SetDialogue(Nextindex); //ROW, 행을 지정해서 대화 세팅.
}
