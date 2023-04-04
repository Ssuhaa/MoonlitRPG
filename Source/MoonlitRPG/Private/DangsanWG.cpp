// Fill out your copyright notice in the Description page of Project Settings.


#include "DangsanWG.h"
#include "DataManager.h"
#include "MagoRewardListWG.h"
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include <UMG/Public/Components/TextBlock.h>
#include <Engine/Texture2D.h>
#include "InventoryComponent.h"
#include <UMG/Public/Components/VerticalBox.h>
#include <UMG/Public/Components/Button.h>

UDangsanWG::UDangsanWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UMagoRewardListWG> TempReward(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_MagoReward.BP_MagoReward_C'"));
	if (TempReward.Succeeded())
	{
		RewardFactory = TempReward.Class;
	}
	for (int32 i = 0; i < 6; i++)
	{
		UMagoRewardListWG* RewardWG = CreateWidget<UMagoRewardListWG>(GetWorld(), RewardFactory);
		RewardWGs.Add(RewardWG);
	}
}

void UDangsanWG::NativeConstruct()
{
	Super::NativeConstruct();
	ButtonBinding();
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		Player->bUIOpen = true;
	}
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	SetDangSanTree();
}

void UDangsanWG::ButtonBinding()
{
	Button_Give->OnPressed.AddUniqueDynamic(this, &UDangsanWG::ClickedGive);
	Button_Close->OnPressed.AddUniqueDynamic(this, &UDangsanWG::ClickedClose);
}

void UDangsanWG::ClickedGive()
{	
	int32 Result = Player->InvenComp->FindItem(MagopieceIndex);
	if (Result > -1)
	{
		int32 GiveAmount = Player->InvenComp->GiveAllItem(MagopieceIndex);
		DangSanData->currMago = GiveAmount;
		if (DangSanData->currMago > DangSanData->MaxMago)
		{
			int32 difference = DangSanData->currMago - DangSanData->MaxMago;
			DangSanData->currMago = DangSanData->MaxMago;
			Player->DangSanLevelUp(DangSanData->RewardStamina);
			SetDangSanTree();
			DangSanData->currMago = difference;
		}
		SetDangSanTree();
	}
	
}

void UDangsanWG::ClickedClose()
{
	RemoveWG();
}

void UDangsanWG::SetDangSanTree()
{
	if(Player == nullptr && DataManager == nullptr) return;

	int32 DangsanDataIndex = Player->DansanLevel - 1;
	DangSanData = DataManager->DangsanData[DangsanDataIndex];

	TB_MagoLevel->SetText(FText::AsNumber(Player->DansanLevel));
	TB_CurrMago->SetText(FText::AsNumber(DangSanData->currMago));
	TB_MaxMago->SetText(FText::AsNumber(DangSanData->MaxMago));

	CurrInvenMagoUpdate();

	FText Name;
	VB_Reward->ClearChildren();
	Name = FText::FromString(TEXT("스태미나"));
	VB_Reward->AddChild(RewardWGs[0]);
	RewardWGs[0]->SetMagoRewardList(StanimaIcon, Name, DangSanData->RewardStamina);
	Name = FText::FromString(TEXT("EXP"));
	VB_Reward->AddChild(RewardWGs[1]);
	RewardWGs[1]->SetMagoRewardList(EXPIcon, Name, DangSanData->RewardEXP);
	Name = FText::FromString(TEXT("화폐"));
	VB_Reward->AddChild(RewardWGs[2]);
	RewardWGs[2]->SetMagoRewardList(MoneyIcon, Name, DangSanData->RewardMoney);

	for (int32 i = 0; i < DangSanData->RewardItems.Num(); i++)
	{
		VB_Reward->AddChild(RewardWGs[i + 3]);
		FIteminfo* rewardItem = DataManager->itemList[DangSanData->RewardItems[i].RewardItem];
		RewardWGs[i + 3]->SetMagoRewardList(rewardItem->itemImage, FText::FromString(rewardItem->ItemName), DangSanData->RewardItems[i].Amount);
	}

}


void UDangsanWG::RemoveWG()
{
	if (Player != nullptr)
	{
		Player->bUIOpen = false;
	}
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	RemoveFromParent();
}

void UDangsanWG::CurrInvenMagoUpdate()
{
	int32 Result = Player->InvenComp->FindItem(MagopieceIndex);
	if (Result > -1)
	{
		TB_InvenMago->SetText(FText::AsNumber(Player->InvenComp->invenItemArr[Result].itemAmount));
	}
	else
	{
		TB_InvenMago->SetText(FText::AsNumber(0));
	}
}
