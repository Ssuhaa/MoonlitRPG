// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_GetItemUI.h"
#include <UMG/Public/Components/Overlay.h>
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/VerticalBox.h>
#include "SH_Player.h"
#include "PlayerMainWG.h"

void UIH_GetItemUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(ItemGetAnim);
	currTime = 0;
}

void UIH_GetItemUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	currTime += InDeltaTime;

	if (currTime > 1.25)
	{
		RemoveFromParent();
	}
}