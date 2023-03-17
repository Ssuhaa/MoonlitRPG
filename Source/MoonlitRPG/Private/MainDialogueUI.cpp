// Fill out your copyright notice in the Description page of Project Settings.


#include "MainDialogueUI.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "NPCBase.h"

void UMainDialogueUI::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	player->DisableInput(player->playerCon);
	player->playerCon->bShowMouseCursor = true;
	
	btn_Close->OnPressed.AddUniqueDynamic(this, &UMainDialogueUI::CloseButton);
}

void UMainDialogueUI::CloseButton()
{
	npc->bTalking = false;
	npc = nullptr;
	player->EnableInput(player->playerCon);
	player->playerCon->bShowMouseCursor = false;
	RemoveFromParent();
}
