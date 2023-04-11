// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataManager.h"
#include "QuestComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOONLITRPG_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<class UInputAction*> inputArray;

	void QuestUIOpen();
	TSubclassOf<class UQuestWG> QuestFactory;
	UPROPERTY()
	class UQuestWG* QuestWG;

	UPROPERTY()
	class ASH_Player* Player;

	UPROPERTY()
	class ADataManager* DataManager;

	UPROPERTY()
	TArray<class AQuestNaviActor*> QuestNavis;

	void GiveQuestReward(FQuestInfo* Questinfo);

public:	
	// Called every frame
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);


	UPROPERTY(EditAnywhere)
	int32 MainQuestIDX = -1;
	
	FQuestInfo* MainQuest;

	void RandomTodayQuest();

	void CompleteMainQuest();
	bool CheackRequirementTarget(int32 index);
	bool isDoneQuestRequirements(FQuestInfo* Questinfo);
	void NaviClear();
	void SetQuestNavi(int32 NaviIndex, AActor* Target);

	

};