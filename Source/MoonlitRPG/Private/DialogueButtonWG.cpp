// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueButtonWG.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include "MainDialogueUI.h"
#include "NPCBase.h"
#include "SH_Player.h"
#include "QuestComponent.h"
#include "DataManager.h"
#include <Kismet/GameplayStatics.h>

void UDialogueButtonWG::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Click->OnPressed.AddUniqueDynamic(this, &UDialogueButtonWG::ClickedButton);

	player = Cast<ASH_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
}

void UDialogueButtonWG::SetText(FString Text)
{
	Text_SelectLog->SetText(FText::FromString(Text));
}

void UDialogueButtonWG::ClickedButton()
{
	DialogueWG->OnClikedNextButton(NextIndex);
	if (NextIndex == 18)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 1.5f, FColor::Black, false, true);
		player->FadeInOut(true);
		player->QuestComp->NaviClear();

		FTimerHandle timer;
		GetWorld()->GetTimerManager().SetTimer(timer, this, &UDialogueButtonWG::TeleportILsub, 3.0f, false);

	}
}

void UDialogueButtonWG::TeleportILsub()
{
	DialogueWG->npc->SetActorLocation(FVector(-67238,-31936, 875));
	player->SetActorLocation(FVector(-71294.0, -48817.0, 286.0));
	DataManager->NavigateTarget(*player->QuestComp->MainQuest);
	DialogueWG->npc->SetActorRotation(FRotator(0, -120, 0));
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 1.5f, FColor::Black, false, true);
	player->FadeInOut(false);
}
