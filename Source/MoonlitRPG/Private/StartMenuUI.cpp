// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuUI.h"
#include <UMG/Public/Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

void UStartMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Start->OnPressed.AddUniqueDynamic(this, &UStartMenuUI::ClickStart);
	btn_Quit->OnPressed.AddUniqueDynamic(this, &UStartMenuUI::ClickQuit);
}

void UStartMenuUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (gameStart)
	{
		currTime += InDeltaTime;
		if (currTime > 0.2)
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));
			currTime = 0;
		}
	}
}

void UStartMenuUI::ClickStart()
{
	gameStart = true;
}

void UStartMenuUI::ClickQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}