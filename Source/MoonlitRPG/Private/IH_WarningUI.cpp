// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_WarningUI.h"

void UIH_WarningUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void UIH_WarningUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	currTime += InDeltaTime;
	
	if (currTime > 2)
	{
		RemoveFromParent();
	}
}