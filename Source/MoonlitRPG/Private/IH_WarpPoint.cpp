﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_WarpPoint.h"
#include "SH_Player.h"
#include <Kismet/KismetMathLibrary.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "IH_WarpActiveUI.h"
#include "IH_InteractionUI.h"
#include <UMG/Public/Components/TextBlock.h>

AIH_WarpPoint::AIH_WarpPoint()
{
	ConstructorHelpers::FClassFinder<UIH_WarpActiveUI>tempActive(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_WarpActive.WG_WarpActive_C'"));
	if (tempActive.Succeeded())
	{
		warpUIFactory = tempActive.Class;
	}
}

void AIH_WarpPoint::BeginPlay()
{
	Super::BeginPlay();

	warpUI = CreateWidget<UIH_WarpActiveUI>(GetWorld(), warpUIFactory);
	
	UIH_InteractionUI* interactionUI = Cast<UIH_InteractionUI>(compInteractWidget->GetUserWidgetObject());
	
	interactionUI->txt_Interaction->SetText(FText::FromString(TEXT("워프 포인트")));
}

void AIH_WarpPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector targetVector = player->GetActorLocation() - GetActorLocation();
	float dot = FVector::DotProduct(GetActorForwardVector(), targetVector.GetSafeNormal());
	float degree = UKismetMathLibrary::DegAcos(dot);
	float distance = FVector::Distance(player->GetActorLocation(), GetActorLocation());

	if (!bsavePoint)
	{
		if (degree < 180 && distance < 300)
		{
			compInteractWidget->SetVisibility(true);
		}
		else
		{
			compInteractWidget->SetVisibility(false);
		}
	}
	else
	{
		compInteractWidget->SetVisibility(false);
	}
}

void AIH_WarpPoint::Interaction()
{
	if (!bsavePoint)
	{
		bsavePoint = true;
		warpUI->AddToViewport();

		FTimerHandle timer;
		GetWorld()->GetTimerManager().SetTimer(timer, this, &AIH_WarpPoint::RemoveUI, 2.0f, false);
	}
}

void AIH_WarpPoint::RemoveUI()
{
	if (warpUI != nullptr)
	{
		warpUI->RemoveFromParent();
	}
}
