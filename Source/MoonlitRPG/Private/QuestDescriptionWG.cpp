// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDescriptionWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/HorizontalBox.h>
#include <UMG/Public/Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include "QuestComponent.h"
#include "QuestSlotWG.h"
#include "QuestNaviActor.h"
#include "SH_Player.h"
#include "NPCBase.h"
#include "DataManager.h"

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
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	
}

void UQuestDescriptionWG::NativeConstruct()
{
	Super::NativeConstruct();
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
	Button_Navi->OnPressed.AddUniqueDynamic(this, &UQuestDescriptionWG::OnPressedNavi);
}

void UQuestDescriptionWG::OnPressedNavi()
{
	Player->QuestComp->NaviClear();
	DataManager->NavigateTarget(*CurrQuest);
}

void UQuestDescriptionWG::SetQuestDescription(FQuestInfo* QuestInfo)
{
	CurrQuest = QuestInfo;
	TB_QName->SetText(FText::FromString(QuestInfo->QuestName));
	TB_QLocation->SetText(FText::FromString(QuestInfo->LocationName));
	TB_QDescription->SetText(FText::FromString(QuestInfo->Description));
	TB_QMoney->SetText(FText::AsNumber(QuestInfo->Reward.RewardMoney));
	TB_QEXP->SetText(FText::AsNumber(QuestInfo->Reward.RewardEXP));

	HB_Reward->ClearChildren();
	for (int32 i = 0; i < QuestInfo->Reward.RewardItems.Num(); i++)
	{
		FInvenItem RewardItem;
		RewardItem.ItemInfoIndex = QuestInfo->Reward.RewardItems[i].RewardItem;
		RewardItem.itemAmount = QuestInfo->Reward.RewardItems[i].Amount;
		RewardSlots[i]->UpdateSlot(RewardItem);
		HB_Reward->AddChild(RewardSlots[i]);
 	}
}