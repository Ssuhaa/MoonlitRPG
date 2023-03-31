// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSummaryWG.h"
#include <UMG/Public/Animation/WidgetAnimation.h>

void UQuestSummaryWG::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(ChangeSummary);
}
