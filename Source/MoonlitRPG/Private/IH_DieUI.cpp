// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_DieUI.h"
#include <UMG/Public/Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include <GameFramework/PlayerController.h>
#include "IH_WarpPoint.h"

void UIH_DieUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	player = Cast<ASH_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!btn_Revive->OnPressed.IsBound())
	{
		btn_Revive->OnPressed.AddDynamic(this, &UIH_DieUI::ReviveButton);
	}

	PlayAnimation(PlayerDie);
}

void UIH_DieUI::ReviveButton()
{
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UIH_DieUI::CallRevive, 3.0f, false);
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 1.5f, FColor::Black, true, true);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	RemoveFromParent();
}

void UIH_DieUI::CallRevive()
{
	FindWarpPoint();
	player->RevivePlayer();
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 1.5f, FColor::Black, true, true);
}

void UIH_DieUI::FindWarpPoint()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);

	for (int32 i = 0; i < actors.Num(); i++)
	{
		if (actors[i]->GetName().Contains(TEXT("WarpPoint")))
		{
			AIH_WarpPoint* warpPointActor = Cast<AIH_WarpPoint>(actors[i]);
			if (warpPointActor->bsavePoint && warpPointActor != nullptr)
			{
				float distance = FVector::Distance(warpPointActor->GetActorLocation(), player->GetActorLocation());
				if (minDist > distance)
				{
					minDist = distance;
					player->warpPoint = warpPointActor;
				}
			}
		}
	}
}