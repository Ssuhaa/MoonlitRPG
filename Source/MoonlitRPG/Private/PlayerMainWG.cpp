// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainWG.h"
#include <UMG/Public/Components/ProgressBar.h>
#include <UMG/Public/Components/Button.h>
#include "SH_Player.h"
#include "InventoryComponent.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Animation/WidgetAnimation.h>
#include "AttackComponent.h"
#include "QuestSummaryWG.h"



UPlayerMainWG::UPlayerMainWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	QuestSummary = CreateWGClass<UQuestSummaryWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_QuestSummary.WG_QuestSummary_C'"));
}

template<typename T>
T* UPlayerMainWG::CreateWGClass(FString path)
{
	TSubclassOf<T> WGFactory;
	ConstructorHelpers::FClassFinder<T> tempWG(*path);
	if (tempWG.Succeeded())
	{
		WGFactory = tempWG.Class;
	}
	return CreateWidget<T>(GetWorld(), WGFactory);
}

void UPlayerMainWG::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		UpdateHP(Player->PlayercurrHP, Player->PlayerTotalHP);
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

	
	if (Player->AttackComp->currWeapon != EWeaponType::None)
	{
		if (!Text_E->IsVisible())
		{
			VisibleSkillText(false);		// false일 때 Text가 보임
		}
	}
	else
	{
		if (Text_E->IsVisible())
		{
			VisibleSkillText(true);
		}
	}
}

void UPlayerMainWG::UpdateQuestSummary(FQuestInfo CurrQuset)
{
	QuestSummaryBox->ClearChildren();
	QuestSummary->QusetName->SetText(FText::FromString(CurrQuset.QuestName));
	QuestSummary->Summary->SetText(FText::FromString(CurrQuset.Summary));
	QuestSummaryBox->AddChild(QuestSummary);
}

void UPlayerMainWG::RemoveSummary()
{
	QuestSummaryBox->ClearChildren();
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

void UPlayerMainWG::UpdateHP(float CurrHP, float TotalHP)
{

	Text_CurrHP->SetText(FText::AsNumber(CurrHP));
	Text_TotalHP->SetText(FText::AsNumber(TotalHP));
	HPBar->SetPercent(CurrHP / TotalHP);
	
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

void UPlayerMainWG::VisibleSkillText(bool isHand)
{
	if (isHand)
	{
		Text_E->SetVisibility(ESlateVisibility::Hidden);
		Text_Q->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Text_E->SetVisibility(ESlateVisibility::Visible);
		Text_Q->SetVisibility(ESlateVisibility::Visible);
	}
}