// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_LoadingUI.h"

void UIH_LoadingUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(LoadingAnim);
}
