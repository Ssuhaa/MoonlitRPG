// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDescriptionWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include "QuestComponent.h"
#include "QuestSlotWG.h"
#include <UMG/Public/Components/HorizontalBox.h>
#include <UMG/Public/Components/Button.h>
#include "QuestNaviActor.h"
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"

UQuestDescriptionWG::UQuestDescriptionWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UQuestSlotWG> tempslot(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_QuestSlot.BP_QuestSlot_C'"));
	if (tempslot.Succeeded())
	{
		SlotFactory = tempslot.Class; 
	}

	for (int32 i = 0; i < 5; i++)
	{
		UQuestSlotWG* currSlot = CreateWidget<UQuestSlotWG>(GetWorld(), SlotFactory);
		RewardSlots.Add(currSlot);
	}
}

void UQuestDescriptionWG::NativeConstruct()
{
	Super::NativeConstruct();
	
	Button_Navi->OnPressed.AddUniqueDynamic(this, &UQuestDescriptionWG::OnPressedNavi);
}

void UQuestDescriptionWG::OnPressedNavi()
{
// 	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
// 	if(Player != nullptr)
// 	{
// 		for (int32 i = 0; i < Quest->Requirements.Num(); i++)
// 		{
// 			AActor* requirement = UGameplayStatics::GetActorOfClass(GetWorld(), Quest->Requirements[i].Requirement);
// 			if (requirement != nullptr)
// 			{
// 				Player->QuestComp->QuestNavis[i]->SetActorLocation(requirement->GetActorLocation());
// 				Player->QuestComp->QuestNavis[i]->SetActiveNaviWG(true);
// 			}
// 			
// 		}
// 	}
}

void UQuestDescriptionWG::SetQuestDescription(FQuestInfo* QuestInfo)
{
	Quest = QuestInfo;
	TB_QName->SetText(FText::FromString(QuestInfo->QuestName));
	TB_QLocation->SetText(FText::FromString(QuestInfo->LocationName));
	TB_QDescription->SetText(FText::FromString(QuestInfo->Description));
	TB_QMoney->SetText(FText::AsNumber(QuestInfo->Reward.RewardMoney));
	TB_QEXP->SetText(FText::AsNumber(QuestInfo->Reward.RewardEXP));

	HB_Reward->ClearChildren();
// 	for (int32 i = 0; i < QuestInfo->Reward.RewardItem.Num(); i++)
// 	{
// 		RewardSlots[i]->UpdateSlot(QuestInfo->Reward.RewardItem[i]);
// 		HB_Reward->AddChild(RewardSlots[i]);
// 	}
}