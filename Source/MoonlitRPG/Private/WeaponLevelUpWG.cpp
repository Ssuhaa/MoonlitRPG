// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponLevelUpWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/ScrollBox.h>
#include <UMG/Public/Components/HorizontalBox.h>
#include <UMG/Public/Components/ProgressBar.h>
#include <Kismet/GameplayStatics.h>
#include "LevelUpSlotWG.h"
#include "NeedItemSelectWG.h"
#include "SH_Player.h"
#include "InventoryWG.h"
#include "OutfitWG.h"


UWeaponLevelUpWG::UWeaponLevelUpWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NeedSelectWG = CreateWGClass<UNeedItemSelectWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_NeedItemSelect.WG_NeedItemSelect_C'"));
	for (int32 i = 0; i <15; i++)
	{
		ULevelUpSlotWG* currWG = CreateWGClass<ULevelUpSlotWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_LevelUpSlot.WG_LevelUpSlot_C'"));
		LevelUpSlots.Add(currWG);
	}
}

template<typename T>
T* UWeaponLevelUpWG::CreateWGClass(FString path)
{
	TSubclassOf<T> WGFactory;
	ConstructorHelpers::FClassFinder<T> tempWG(*path);
	if (tempWG.Succeeded())
	{
		WGFactory = tempWG.Class;
	}
	return CreateWidget<T>(GetWorld(), WGFactory);
}

void UWeaponLevelUpWG::NativeConstruct()
{
	Super::NativeConstruct();
	ButtonBinding();
}


void UWeaponLevelUpWG::NativeDestruct()
{
	Super::NativeDestruct();
	NeedSelectWG->RemoveFromParent();
}

void UWeaponLevelUpWG::UpdateLevelUpWG()
{
	TB_CurrLevel->SetText(FText::AsNumber(inventoryData.invenitem.WeaponData.Level));
	TB_currEXP->SetText(FText::AsNumber(inventoryData.invenitem.WeaponData.CurrEXP));
	TB_MaxEXP->SetText(FText::AsNumber(inventoryData.invenitem.WeaponData.MaxEXP));
	TB_CurrPower->SetText(FText::AsNumber(inventoryData.invenitem.WeaponData.CurrPower));
	TB_UpgradePower->SetText(FText::AsNumber(inventoryData.invenitem.WeaponData.CurrPower+ inventoryData.itemGradeData.PlusPower));

	float CurrEXP = float(inventoryData.invenitem.WeaponData.CurrEXP) / float(inventoryData.invenitem.WeaponData.MaxEXP);
	Progress_EXP->SetPercent(CurrEXP);

	//15개의 비어있는 슬랏을 리스트에 올린다.
	SelectListBox->ClearChildren();
	for (int32 i = 0; i < LevelUpSlots.Num(); i++)
	{
		if (LevelUpSlots[i]->isFill)
		{
			LevelUpSlots[i]->ResetSlot();
		}
		SelectListBox->AddChild(LevelUpSlots[i]);
		LevelUpSlots[i]->WeaponLevelUpWG = this;
		LevelUpSlots[i]->NeedSelectWG = NeedSelectWG;
	
	}
	//무기선택창에 정보를 전달.
	NeedSelectWG->inventoryData = inventoryData;
	NeedSelectWG->LevelupSlots = LevelUpSlots;


}

void UWeaponLevelUpWG::ReceiveSelectSlotData(FinvenData invenData)
{
	inventoryData = invenData;
	invenData.invenitem.SendLevelUpClear.AddUObject(this, &UWeaponLevelUpWG::UpdateLevelUpWG);
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	UpdateLevelUpWG();
}

void UWeaponLevelUpWG::UpdateUseMoney()
{
	SetCount = 0;
	ToTalEXP = 0;
	CurrUseMoney = 0;
	UseTempItems.Empty();

	for (int32 i = 0 ; i < LevelUpSlots.Num() ; i++)
	{
		if (LevelUpSlots[i]->isFill)
		{
			SetCount++;
			UseTempItems.Add(LevelUpSlots[i]->invenData.invenitem);
			ToTalEXP += LevelUpSlots[i]->invenData.itemGradeData.PlusEXP * LevelUpSlots[i]->invenData.invenitem.WeaponData.Level;
		}
	}

	CurrUseMoney = SetCount * 120;
	TB_levelUpMoney->SetText(FText::AsNumber(CurrUseMoney));
}

void UWeaponLevelUpWG::ButtonBinding()
{
	Button_LevelUp->OnPressed.AddUniqueDynamic(this, &UWeaponLevelUpWG::LevelUp);
}

void UWeaponLevelUpWG::LevelUp() //레벨업 버튼을 누르면 실행.
{
	if (Player != nullptr)
	{
		int32 index = Player->InvenComp->FindItem(inventoryData.invenitem);
		if(index < 0 || SetCount <= 0) return;
		bool result = Player->InvenComp->invenItemArr[index].PlusCurrEXP(ToTalEXP, SetCount, &Player->InvenComp->Money, inventoryData.itemGradeData);
		if (result)
		{
			inventoryData.invenitem = Player->InvenComp->invenItemArr[index];
			SendUseditem();
			OutfitWG->ReceiveUseItem(inventoryData.invenitem);
		}

	}
}

void UWeaponLevelUpWG::SendUseditem()
{
	for (int32 i = 0; i < UseTempItems.Num(); i++)
	{
		Player->InvenComp->MinusItemAmount(UseTempItems[i], 1);
	}
	for (int32 i = 0; i < LevelUpSlots.Num(); i++)
	{
		if (LevelUpSlots[i]->isFill)
		{
			LevelUpSlots[i]->ResetSlot();
		}
	}
	NeedSelectWG->SetNeedItemSelectWG();
	NeedSelectWG->UsedItemSlotClear();
}
