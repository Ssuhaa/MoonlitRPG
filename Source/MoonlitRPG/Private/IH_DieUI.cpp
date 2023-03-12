// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_DieUI.h"
#include <UMG/Public/Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include <GameFramework/PlayerController.h>
#include "IH_LoadingUI.h"

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
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UIH_DieUI::PlayerRevive,10.0f, false);
	player->loadingUI->AddToViewport();
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	RemoveFromParent();
}

void UIH_DieUI::PlayerRevive()
{
	player->loadingUI->PlayAnimationReverse(player->loadingUI->LoadingAnim);
	player->RevivePlayer();

	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UIH_DieUI::LoadingRemove, 3.0f, false);
}

void UIH_DieUI::LoadingRemove()
{
	if (player->loadingUI != nullptr)
	{
		player->loadingUI->RemoveFromParent();
	}
}