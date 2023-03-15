// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_InteractionUI.h"

void UIH_InteractionUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayOpenAnim();
}

void UIH_InteractionUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UIH_InteractionUI::PlayOpenAnim()
{
	PlayAnimation(OpenAnim);
	PlayAnimation(PointerAnim, 0.0f, 0);
}