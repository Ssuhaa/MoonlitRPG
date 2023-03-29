// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenShotUI.h"
#include <UMG/Public/Components/Button.h>
#include <Kismet/KismetSystemLibrary.h>

void UScreenShotUI::NativeConstruct()
{
	btn_ScreenShot->OnPressed.AddUniqueDynamic(this, &UScreenShotUI::ScreenShot);
}

void UScreenShotUI::ScreenShot()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("HighResShot 2"));
}