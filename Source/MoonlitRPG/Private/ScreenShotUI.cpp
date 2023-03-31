// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenShotUI.h"
#include <UMG/Public/Components/Button.h>
#include <Kismet/KismetSystemLibrary.h>
#include <HighResScreenshot.h>

void UScreenShotUI::NativeConstruct()
{
	Super::NativeConstruct();

	btn_ScreenShot->OnPressed.AddUniqueDynamic(this, &UScreenShotUI::TakeScreenShot);
}

void UScreenShotUI::TakeScreenShot()
{
	PlayAnimation(ScreenshotAnim);
	ScreenShot("MyScreenShot");
}

void UScreenShotUI::ScreenShot(FString FileName)
{
	//UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("HighResShot 2"));

	FString ScreenShotPath = FPaths::ScreenShotDir() + FileName + TEXT(".png");
	FViewport* Viewport = GEngine->GameViewport->Viewport;

	if (!Viewport)
	{
		return;
	}
	else
	{
		FHighResScreenshotConfig& Config = GetHighResScreenshotConfig();
		Config.bCaptureHDR = false;
		Config.bMaskEnabled = false;
		Config.bDumpBufferVisualizationTargets = false;

		Viewport->TakeHighResScreenShot();
	}
}