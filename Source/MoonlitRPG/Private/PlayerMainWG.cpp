// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainWG.h"
#include <UMG/Public/Components/ProgressBar.h>
#include <UMG/Public/Components/Button.h>
#include "SH_Player.h"
#include "InventoryComponent.h"
#include <Kismet/GameplayStatics.h>



void UPlayerMainWG::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		Button_backpack->OnClicked.AddDynamic(Player, & ASH_Player::onClickedBackPack);
	}

}
void UPlayerMainWG::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

	Super::NativeTick(MyGeometry,InDeltaTime);

	if (StaminaBar->GetPercent() == 1.0f)
	{
		currentTime += InDeltaTime;
		if (currentTime > 2)
		{
			VisibleStaminaBar(false);
			currentTime = 0;
		}
	}
	else
	{
		VisibleStaminaBar(true);
	}
}

void UPlayerMainWG::UpdateStamina(float Stamina, float MaxStamina)
{
	float percent = Stamina/MaxStamina;	
	StaminaBar->SetPercent(percent);
}

void UPlayerMainWG::VisibleStaminaBar(bool isUseStamina)
{
	if (isUseStamina)
	{
		StaminaBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		StaminaBar->SetVisibility(ESlateVisibility::Hidden);
	}
	
}
