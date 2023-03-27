// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetActorBase.h"
#include "QuestNaviActor.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AQuestNaviActor : public AWidgetActorBase
{
	GENERATED_BODY()
		AQuestNaviActor();
protected:
	virtual void BeginPlay() override;

	TSubclassOf<class UQuestNaviWG> NaviFactory;
	class UQuestNaviWG* NaviWG;

public: 

	void SetActiveNaviWG(bool value);
};
