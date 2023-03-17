// Fill out your copyright notice in the Description page of Project Settings.


#include "MainDialogueUI.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>
#include <GameFramework/CharacterMovementComponent.h>

void UMainDialogueUI::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	player->DisableInput(player->playerCon);
	player->playerCon->bShowMouseCursor = true;
	
	btn_Close->OnPressed.AddUniqueDynamic(this, &UMainDialogueUI::CloseButton);

	PlayAnimation(DialogueOpenAnim);
}

void UMainDialogueUI::CloseButton()
{
	if (npc != nullptr)
	{
		npc->bTalking = false;
		npc = nullptr;
	}

	player->playerCon->bShowMouseCursor = false;
	player->EnableInput(player->playerCon);
	RemoveFromParent();
}
