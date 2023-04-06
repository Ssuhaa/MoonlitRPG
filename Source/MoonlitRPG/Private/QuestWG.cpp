// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>
#include "QuestComponent.h"
#include "QuestListButtonWG.h"
#include <UMG/Public/Components/VerticalBox.h>
#include "QuestDescriptionWG.h"
#include <UMG/Public/Components/ScaleBox.h>


UQuestWG::UQuestWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UQuestListButtonWG> tempList(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_QuestList.WG_QuestList_C'"));
	if (tempList.Succeeded())
	{
		WGFactory.Add(tempList.Class); //0¹ø QuestList À§Á¬
	}
	ConstructorHelpers::FClassFinder <UQuestDescriptionWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_QuestDescription.WG_QuestDescription_C'"));
	if (tempWG.Succeeded())
	{
		WGFactory.Add(tempWG.Class); //1¹ø Description À§Á¬
	}

	for (int32 i = 0; i < 30; i++)
	{
		UQuestListButtonWG* CurrList = CreateWidget<UQuestListButtonWG>(GetWorld(), WGFactory[0]);
		QuestButtons.Add(CurrList);
	}
	
	QuestDescription = CreateWidget<UQuestDescriptionWG>(GetWorld(), WGFactory[1]);
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
}

void UQuestWG::NativeConstruct()
{
	Super::NativeConstruct();
	
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		Player->bUIOpen = true;
	}
	ButtonBinding();
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	UpdateQuestList();
}

void UQuestWG::NativeDestruct()
{
	Super::NativeDestruct();

}

void UQuestWG::ButtonBinding()
{
	Button_All->OnPressed.AddUniqueDynamic(this, &UQuestWG::ClickedAllButton);
	Button_Main->OnPressed.AddUniqueDynamic(this, &UQuestWG::ClickedMainButton);
	Button_Sub->OnPressed.AddUniqueDynamic(this, &UQuestWG::ClickedSubButton);
	Button_Today->OnPressed.AddUniqueDynamic(this, &UQuestWG::ClickedTodayButton);
	Button_Close->OnPressed.AddUniqueDynamic(this, &UQuestWG::ClickedCloseButton);
}

void UQuestWG::ClickedAllButton()
{
	ChangeQuestList(EQuestType::Total);
}

void UQuestWG::ClickedMainButton()
{
	ChangeQuestList(EQuestType::Main);
}

void UQuestWG::ClickedSubButton()
{
	ChangeQuestList(EQuestType::Sub);
}

void UQuestWG::ClickedTodayButton()
{
	ChangeQuestList(EQuestType::Today);
}

void UQuestWG::ClickedCloseButton()
{
	RemoveQuestWG();
}



void UQuestWG::ChangeQuestList(EQuestType ChangeType)
{
	CurrQuestType = ChangeType;
	UpdateQuestList();
}


void UQuestWG::UpdateQuestList()
{
	VB_QuestList->ClearChildren();
	switch (CurrQuestType)
	{
	case EQuestType::Main:
		SetQuestList(DataManager->MainQuestList);
		break;
	case EQuestType::Today:
		SetQuestList(DataManager->TodayQuestList);
		break;
	case EQuestType::Sub:
		break;
	case EQuestType::Total:
		for (int32 i = 0; i < DataManager->ToTalQuestList.Num(); i++)
		{
			if (DataManager->ToTalQuestList[i]->Queststate != EQuestState::Continue) continue;
			if (!QuestButtons.IsValidIndex(i))
			{
				UQuestListButtonWG* CurrList = CreateWidget<UQuestListButtonWG>(GetWorld(), WGFactory[0]);
				QuestButtons.Add(CurrList);
			}
			QuestButtons[i]->QuestWG = this;
			VB_QuestList->AddChild(QuestButtons[i]);
			QuestButtons[i]->SetQuestListWG(DataManager->ToTalQuestList[i]);
 		}
		break;
	}

	if (VB_QuestList->GetChildrenCount() != 0)
	{
		UQuestListButtonWG* First = Cast<UQuestListButtonWG>(VB_QuestList->GetChildAt(0));
		ReciveSelectQuest(First->SelectQuest);
  	}
}

void UQuestWG::SetQuestList(TArray<TObjectPtr<FQuestInfo>> QuestList)
{
	for (int32 i = 0; i < QuestList.Num(); i++)
	{
		if (QuestList[i]->Queststate != EQuestState::Continue) continue;
		if (!QuestButtons.IsValidIndex(i))
		{
			UQuestListButtonWG* CurrList = CreateWidget<UQuestListButtonWG>(GetWorld(), WGFactory[0]);
			QuestButtons.Add(CurrList);
		}
		QuestButtons[i]->QuestWG = this;
		VB_QuestList->AddChild(QuestButtons[i]);
		QuestButtons[i]->SetQuestListWG(QuestList[i]);
	}
}

void UQuestWG::RemoveQuestWG()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	Player->bUIOpen = false;
	VB_QuestList->ClearChildren();
	SB_Description->ClearChildren();
	RemoveFromParent();
}

void UQuestWG::ReciveSelectQuest(TObjectPtr<FQuestInfo> SelectQuest)
{
	SB_Description->ClearChildren();
	SB_Description->AddChild(QuestDescription);
	QuestDescription->SetQuestDescription(SelectQuest);
}