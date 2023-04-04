// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <Kismet/GameplayStatics.h>
#include "QuestWG.h"
#include "QuestNaviActor.h"
#include "DataManager.h"
#include "SH_Player.h"
#include "MainDialogueUI.h"
#include "InventoryComponent.h"
#include "PlayerMainWG.h"


// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder <UInputAction> tempKey(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_J.Key_J'"));
	if (tempKey.Succeeded())
	{
		inputArray.Add(tempKey.Object);
	}
	ConstructorHelpers::FClassFinder <UQuestWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Quest.WG_Quest_C'"));
	if (tempWG.Succeeded())
	{
		QuestFactory = tempWG.Class;
	}
	
}

// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASH_Player>(GetOwner());
	QuestWG = CreateWidget<UQuestWG>(GetWorld(), QuestFactory);
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));

	for (int32 i = 0; i < 3; i++)
	{
		AQuestNaviActor* CurrNavi = GetWorld()->SpawnActor<AQuestNaviActor>(AQuestNaviActor::StaticClass(), FVector(0), FRotator::ZeroRotator);
		CurrNavi->SetActiveNaviWG(false);
		QuestNavis.Add(CurrNavi);
	}

	//처음 퀘스트
	CompleteMainQuest();
	RandomTodayQuest();
}



void UQuestComponent::GiveQuestReward(FQuestInfo* Questinfo)
{
	for (int32 i = 0; i < Questinfo->Reward.RewardItems.Num(); i++)
	{
		FRewarditem item = Questinfo->Reward.RewardItems[i];
		if (DataManager->itemList[item.RewardItem]->itemType == EItemType::Outfit)
		{
			Player->InvenComp->WeaponAddItemToinven(item.RewardItem, item.WeaponData);
		}
		else
		{
			Player->InvenComp->CommonCheckSameItemAfterAdd(item.RewardItem, item.Amount);
		}
	}
	Player->InvenComp->Money += MainQuest->Reward.RewardMoney;

}

void UQuestComponent::RandomTodayQuest()
{
	TArray<int32> Random;
	TArray<int32> SelectIndex;
	for (int32 i = 0; i < DataManager->TodayQuestList.Num(); i++)
	{
		DataManager->TodayQuestList[i]->Queststate = EQuestState::None;
		Random.Add(i);
	}
	for (int32 i = 0; i < 2; i++)
	{
		int32 value = FMath::RandRange(0, Random.Num()-1);
		SelectIndex.Add(Random[value]);
		Random.RemoveAt(value);
	}

	for (int32 i = 0; i < SelectIndex.Num(); i++)
	{
		DataManager->TodayQuestList[SelectIndex[i]]->Queststate = EQuestState::Continue;
	}

}

void UQuestComponent::CompleteMainQuest()
{
	if (!DataManager->MainQuestList.IsValidIndex(MainQuestIDX +1)) 
	{
		NaviClear();
		Player->MainHUD->RemoveSummary();
		return;
	}

	if (MainQuestIDX > -1) //이후 퀘스트
	{
		if (isDoneQuestRequirements(MainQuest))
		{
			MainQuest->Queststate = EQuestState::Done;
			GiveQuestReward(MainQuest);

			MainQuestIDX++;
			MainQuest = DataManager->MainQuestList[MainQuestIDX];
			MainQuest->Queststate = EQuestState::Continue;
			NaviClear();
			DataManager->NavigateTarget(*MainQuest);
			Player->CompleteQuest(*MainQuest);
			QuestWG->UpdateQuestList();

		}
		
	}
	else // 첫 퀘스트.
	{
		MainQuestIDX++;
		MainQuest = DataManager->MainQuestList[MainQuestIDX];
		MainQuest-> Queststate = EQuestState::Continue;
		NaviClear();
		DataManager->NavigateTarget(*MainQuest);
		Player->CompleteQuest(*MainQuest);
		QuestWG->UpdateQuestList();
	}

}


void UQuestComponent::CheackRequirementTarget(int32 index)
{

	for (int32 i = 0; i <MainQuest->Requirements.Num(); i++)
	{
		if (MainQuest->Requirements[i].Requirementindex == index)
		{
			MainQuest->Requirements[i].isRequirements = true;
		}
	}
}

bool UQuestComponent::isDoneQuestRequirements(FQuestInfo* Questinfo)
{
	for (int32 i = 0; i < Questinfo->Requirements.Num(); i++)
	{
		if (!Questinfo->Requirements[i].isRequirements)
		{
			return false;
		}
	}
	return true;
}



void UQuestComponent::SetQuestNavi(int32 NaviIndex, AActor* Target)
{
	QuestNavis[NaviIndex]->SetActorLocation(Target->GetActorLocation()+ FVector(0,0,100));
	QuestNavis[NaviIndex]->SetActiveNaviWG(true);
}


void UQuestComponent::NaviClear()
{
	for (int32 i = 0; i < QuestNavis.Num(); i++)
	{
		QuestNavis[i]->SetActiveNaviWG(false);
	}
}

// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent)
{
	if (inputArray.IsValidIndex(0))
	{
		EnhancedInputComponent->BindAction(inputArray[0], ETriggerEvent::Triggered, this, &UQuestComponent::QuestUIOpen); // Tab
	}
}


void UQuestComponent::QuestUIOpen()
{

	if (Player->bUIOpen == false && !QuestWG->IsInViewport())
	{
		QuestWG->AddToViewport();
	}
	else
	{
		QuestWG->RemoveQuestWG();
	}
}

