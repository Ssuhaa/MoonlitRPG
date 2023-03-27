// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "QuestWG.h"
#include "QuestNaviActor.h"
#include <Kismet/GameplayStatics.h>
#include "DataManager.h"
#include "SH_Player.h"

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
	// ...

	//처음 퀘스트
	CompleteMainQuest();
}

void UQuestComponent::CompleteMainQuest()
{
	mainQuestIdx++;
	UE_LOG(LogTemp, Error, TEXT("%s"), *DataManager->newQuestList[mainQuestIdx].description);
	currGoalCount = 0;
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

void UQuestComponent::reciveQuest(FQuestInfo sendQuest)
{

	playerQuestList.Add(sendQuest);
}
