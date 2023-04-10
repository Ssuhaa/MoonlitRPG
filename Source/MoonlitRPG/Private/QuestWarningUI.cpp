// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWarningUI.h"

void UQuestWarningUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(WarningAnim);
}

void UQuestWarningUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	currTime += InDeltaTime;

	if (currTime > 2.0f)
	{
		RemoveFromParent();
		currTime = 0;
	}
}