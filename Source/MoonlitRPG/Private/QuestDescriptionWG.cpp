// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDescriptionWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include "QuestComponent.h"
#include "QuestSlotWG.h"
#include <UMG/Public/Components/HorizontalBox.h>

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

void UQuestDescriptionWG::SetQuestDescription(FQuestInfo* QuestInfo)
{
	TB_QName->SetText(FText::FromString(QuestInfo->QuestName));
	TB_QLocation->SetText(FText::FromString(QuestInfo->LocationName));
	TB_QDescription->SetText(FText::FromString(QuestInfo->Description));
	TB_QMoney->SetText(FText::AsNumber(QuestInfo->Reward.RewardMoney));
	TB_QEXP->SetText(FText::AsNumber(QuestInfo->Reward.RewardEXP));

	HB_Reward->ClearChildren();
	for (int32 i = 0; i < QuestInfo->Reward.RewardItem.Num(); i++)
	{
		RewardSlots[i]->UpdateSlot(QuestInfo->Reward.RewardItem[i]);
		HB_Reward->AddChild(RewardSlots[i]);
	}
}