// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainWG.h"
#include <UMG/Public/Components/ProgressBar.h>
#include <UMG/Public/Components/Button.h>
#include "SH_Player.h"
#include "InventoryComponent.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Animation/WidgetAnimation.h>



void UPlayerMainWG::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		UpdateHP(Player->PlayerHP);
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
	StaminaBar->SetFillColorAndOpacity(FLinearColor::LerpUsingHSV(FLinearColor::Red, FLinearColor::Green, percent));

}

void UPlayerMainWG::UpdateEtime(float Etime)
{
	FNumberFormattingOptions NumOption = FNumberFormattingOptions();
	NumOption.MaximumFractionalDigits = 1;
	NumOption.MinimumFractionalDigits = 1;

	if (Etime == 0)
	{
		Text_E->SetColorAndOpacity(FLinearColor::Red);
		Text_ETime->SetText(FText::FromString((TEXT(" "))));
	}
	else
	{
		Text_E->SetColorAndOpacity(FLinearColor::White);
		Text_ETime->SetText(FText::AsNumber(Etime, &NumOption));
	}
}

void UPlayerMainWG::UpdateQPercent(float Qpercent) 
{
	if (Qpercent == 100)
	{
		Text_Q->SetColorAndOpacity(FLinearColor::Red);
		Text_Qpercent->SetText(FText::FromString((TEXT(" "))));
	}
	else if (Qpercent == 0)
	{
		Text_Q->SetColorAndOpacity(FLinearColor::White);
		Text_Qpercent->SetText(FText::FromString((TEXT(" "))));
	}
	else
	{
		Text_Qpercent->SetText(FText::AsNumber(Qpercent));
	}
}

void UPlayerMainWG::UpdateHP(int32 hp)
{
	Text_HP->SetText(FText::AsNumber(hp));
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
