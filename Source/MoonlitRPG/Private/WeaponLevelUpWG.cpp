// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponLevelUpWG.h"
#include <UMG/Public/Components/TextBlock.h>
#include "WeaponItemBase.h"
#include <UMG/Public/Components/Button.h>
#include "LevelUpSlotWG.h"
#include <UMG/Public/Components/ScrollBox.h>
#include "NeedItemSelectWG.h"
#include <UMG/Public/Components/HorizontalBox.h>
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include <UMG/Public/Components/ProgressBar.h>
#include "InventoryWG.h"


UWeaponLevelUpWG::UWeaponLevelUpWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UNeedItemSelectWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_NeedItemSelect.WG_NeedItemSelect_C'"));
	if (tempWG.Succeeded())
	{
		WGFactory.Add(tempWG.Class);
	}
	ConstructorHelpers::FClassFinder<ULevelUpSlotWG> tempWG1(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_LevelUpSlot.WG_LevelUpSlot_C'"));
	if (tempWG1.Succeeded())
	{
		WGFactory.Add(tempWG1.Class);
	}	
	
	NeedSelectWG = CreateWidget<UNeedItemSelectWG>(GetWorld(), WGFactory[0]);

	for (int32 i = 0; i <15; i++)
	{
		ULevelUpSlotWG* currWG = CreateWidget<ULevelUpSlotWG>(GetWorld(), WGFactory[1]);
		LevelUpSlots.Add(currWG);
		
	}
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
	FWeaponinfo* weaponinfo = &SelectedSlot->invenInfo->weaponinfomaiton;
	TB_CurrLevel->SetText(FText::AsNumber(weaponinfo->Level));
	TB_currEXP->SetText(FText::AsNumber(weaponinfo->CurrEXP));
	TB_MaxEXP->SetText(FText::AsNumber(weaponinfo->MaxEXP));
	TB_CurrPower->SetText(FText::AsNumber(weaponinfo->Power));
	TB_UpgradePower->SetText(FText::AsNumber(weaponinfo->Power+weaponinfo->PlusPower));
	float CurrEXP = float(weaponinfo->CurrEXP) / float(weaponinfo->MaxEXP);
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
	NeedSelectWG->SelectedSlot = SelectedSlot;
	NeedSelectWG->LevelupSlots = LevelUpSlots;


}


void UWeaponLevelUpWG::SendUseitem()
{
	for (int32 i = 0; i < UseItems.Num(); i++)
	{
		Player->InvenComp->PlusMinusItemAmount(UseItems[i],-1);
		
	}
	NeedSelectWG->SetNeedItemSelectWG();
	Player->InvenComp->inventory->Setinventory();
}

void UWeaponLevelUpWG::ReceiveSelectSlotData(class UInventorySlotWG* SelectSlot)
{
	SelectedSlot = SelectSlot;
	SelectedSlot->invenInfo->weaponinfomaiton.SendLevelUpClear.AddUObject(this, &UWeaponLevelUpWG::UpdateLevelUpWG);
	UpdateLevelUpWG();
}

void UWeaponLevelUpWG::UpdateUseMoney()
{
	SetCount = 0;
	ToTalEXP = 0;
	UseItems.Empty();
	for (int32 i = 0 ; i < LevelUpSlots.Num() ; i++)
	{
		if (LevelUpSlots[i]->isFill)
		{
			SetCount++;
			UseItems.Add(LevelUpSlots[i]->invenInfo->iteminfomation);
			EItemgrade currtype= LevelUpSlots[i]->invenInfo->iteminfomation.itemgrade;
			int32 AddEXP = 0;
			switch (currtype)
			{
			case EItemgrade::Common:
				AddEXP = 10 * LevelUpSlots[i]->invenInfo->weaponinfomaiton.Level;
				break;
			case EItemgrade::Rare:
				AddEXP = 20 * LevelUpSlots[i]->invenInfo->weaponinfomaiton.Level;
				break;
			case EItemgrade::Unique:
				AddEXP = 40 * LevelUpSlots[i]->invenInfo->weaponinfomaiton.Level;
				break;
			case EItemgrade::Legendary:
				AddEXP = 80 * LevelUpSlots[i]->invenInfo->weaponinfomaiton.Level;
				break;
			default:
				break;
			}

			ToTalEXP +=AddEXP;
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
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (Player != nullptr)
	{
		if (SetCount > 0)
		{
			bool result = SelectedSlot->invenInfo->weaponinfomaiton.PlusCurrEXP(ToTalEXP, SetCount, &Player->InvenComp->Money);
			if (result)
			{
				SendUseitem();
			}
		}

	}
}
