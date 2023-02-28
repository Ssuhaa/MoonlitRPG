// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/Button.h>

void UInventoryWG::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Close->OnClicked.AddDynamic(this, &UInventoryWG::RemoveWidget);
}

void UInventoryWG::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UInventoryWG::RemoveWidget()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UInventoryWG::AddWidget()
{
	AddToViewport();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}
