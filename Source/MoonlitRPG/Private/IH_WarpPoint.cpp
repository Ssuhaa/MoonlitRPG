// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_WarpPoint.h"
#include "SH_Player.h"
#include <Kismet/KismetMathLibrary.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "IH_WarpActiveUI.h"
#include "IH_InteractionUI.h"
#include <UMG/Public/Components/TextBlock.h>
#include "PlayerMainWG.h"
#include <UMG/Public/Components/VerticalBox.h>
#include <Particles/ParticleSystemComponent.h>

AIH_WarpPoint::AIH_WarpPoint()
{
	ConstructorHelpers::FClassFinder<UIH_WarpActiveUI>tempActive(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_WarpActive.WG_WarpActive_C'"));
	if (tempActive.Succeeded())
	{
		warpUIFactory = tempActive.Class;
	}

	activeEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Active Effect"));
	activeEffect->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/GoodFXLevelUp/FX/Particles/PS_GFXLU_Aura.PS_GFXLU_Aura'"));
	if (tempActive.Succeeded())
	{
		activeEffect -> SetTemplate(tempEffect.Object);
	}
}

void AIH_WarpPoint::BeginPlay()
{
	Super::BeginPlay();

	activeEffect->SetActive(false);
	warpUI = CreateWidget<UIH_WarpActiveUI>(GetWorld(), warpUIFactory);
}

void AIH_WarpPoint::Tick(float DeltaTime)
{
	FVector targetVector = player->GetActorLocation() - GetActorLocation();
	float dot = FVector::DotProduct(GetActorForwardVector(), targetVector.GetSafeNormal());
	float degree = UKismetMathLibrary::DegAcos(dot);
	float distance = FVector::Distance(player->GetActorLocation(), GetActorLocation());

	if (!bsavePoint)
	{
		if (degree < 180 && distance < 150)
		{
			if (!player->MainHUD->InteractionBox->GetAllChildren().Contains(interactionUI))
			{
				player->MainHUD->InteractionBox->AddChildToVerticalBox(interactionUI);
			}
		}
		else
		{
			if (player->MainHUD->InteractionBox->GetAllChildren().Contains(interactionUI))
			{
				player->MainHUD->InteractionBox->RemoveChild(interactionUI);
			}
		}
	}
}

void AIH_WarpPoint::Interaction()
{
	if (!bsavePoint)
	{
		bsavePoint = true;
		warpUI->AddToViewport();
		activeEffect->SetActive(true);

		FTimerHandle timer;
		GetWorld()->GetTimerManager().SetTimer(timer, this, &AIH_WarpPoint::RemoveUI, 2.0f, false);

		if (interactionUI != nullptr)
		{
			interactionUI->RemoveFromParent();
		}
	}

}

void AIH_WarpPoint::RemoveUI()
{
	if (warpUI != nullptr)
	{
		warpUI->RemoveFromParent();
	}
}
