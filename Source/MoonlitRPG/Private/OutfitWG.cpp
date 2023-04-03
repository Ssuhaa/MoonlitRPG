// Fill out your copyright notice in the Description page of Project Settings.


#include "OutfitWG.h"
#include <UMG/Public/Components/ScaleBox.h>
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Border.h>
#include <UMG/Public/Components/CanvasPanel.h>
#include <Kismet/GameplayStatics.h>
#include "ItemDescriptionWG.h"
#include "InventorySlotWG.h"
#include "WeaponUpgradeWG.h"
#include "WeaponLevelUpWG.h"
#include "InventoryWG.h"
#include "SH_Player.h"
#include "PreviewActor.h"

UOutfitWG::UOutfitWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	description = CreateWGClass<UItemDescriptionWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_ItemDescription.BP_ItemDescription_C'"));
	UpgradeWG = CreateWGClass<UWeaponUpgradeWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_WeaponUpgrade.WG_WeaponUpgrade_C'"));
	LevelUpWG = CreateWGClass<UWeaponLevelUpWG>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_WeaponLevelUp.WG_WeaponLevelUp_C'"));
}

template<typename T>
T* UOutfitWG::CreateWGClass(FString path)
{
	TSubclassOf<T> WGFactory;
	ConstructorHelpers::FClassFinder<T> tempWG(*path);
	if (tempWG.Succeeded())
	{
		WGFactory = tempWG.Class;
	}
	return CreateWidget<T>(GetWorld(), WGFactory);
}


void UOutfitWG::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonBinding();
	ReinforceSwitch(EEquipmentState::Detail);
	OutfitActor = GetWorld()->SpawnActor<APreviewActor>(APreviewActor::StaticClass(), FVector(0, 0, 10000), FRotator(0,180,0));
	OutfitActor->SetPreviewMesh(inventoryData.Weaponinfo.Mesh);
}

void UOutfitWG::NativeDestruct()
{
	Super::NativeDestruct();
	OutfitActor->Destroy();
	SendToInvenInfo.Clear();
	player->InvenComp->inventory->Setinventory();
}

void UOutfitWG::ButtonBinding()
{
	SendToInvenInfo.AddUObject(description, &UItemDescriptionWG::ReceiveSelectSlotData);
	SendToInvenInfo.AddUObject(UpgradeWG, &UWeaponUpgradeWG::SetUpGradeWG);
	SendToInvenInfo.AddUObject(LevelUpWG, &UWeaponLevelUpWG::ReceiveSelectSlotData);

	SendToInvenInfo.Broadcast(inventoryData);

	Button_Detail->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedDetail);
	Button_LevelUp->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedLevelUp);
	Button_Upgrade->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedUpgrade);
	Button_Change->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedWearing);
	Button_Off->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedOff);
	Button_Back->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedBack);
	UpgradeWG->Button_Upgrade->OnPressed.AddUniqueDynamic(this, &UOutfitWG::UpdateOutfitWG);
}


void UOutfitWG::ReceiveSelectSlotData(FinvenData invenData)
{
	inventoryData = invenData;
	inventoryData.invenitem.SendLevelUpClear.AddUObject(this, &UOutfitWG::UpdateOutfitWG);
	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));

	SetOutfItWG();
	UpdateOutfitWG();
}

void UOutfitWG::UpdateOutfitWG()
{
	//현재레벨에 따라서 강화/ 돌파버튼 스위치 업데이트
	inventoryData.invenitem.WeaponData.Level == inventoryData.invenitem.WeaponData.MaxLevel ? ButtonSwitch(true) : ButtonSwitch(false);
	
	//플레이어의 현재 잔액 업데이트
	if (player != nullptr)
	{
		Text_Money->SetText(FText::AsNumber(player->InvenComp->Money));
	}
}


void UOutfitWG::SetOutfItWG()
{
	UpgradeWG->OutfitWG = this;
	LevelUpWG->OutfitWG = this;

	ReinforceSwitch(EEquipmentState::Detail);

	itemDescription->AddChild(description);

	FText weaponType;
	switch (int32(inventoryData.Weaponinfo.WeaponType))
	{
	case 1:
		weaponType = FText::FromString(TEXT("두손 검"));
		break;
	case 2:
		weaponType = FText::FromString(TEXT("단 검"));
		break;
	case 3:
		weaponType = FText::FromString(TEXT("활"));
		break;
	}
	Text_WeaponType->SetText(weaponType);

	FText itemname = FText::FromString(inventoryData.iteminfo.ItemName);
	Text_WeaponName->SetText(itemname);
}

void UOutfitWG::OnclickedDetail()
{
	ReinforceSwitch(EEquipmentState::Detail);
}
void UOutfitWG::OnclickedLevelUp()
{
	ReinforceSwitch(EEquipmentState::LevelUp);
}

void UOutfitWG::OnclickedUpgrade()
{
	ReinforceSwitch(EEquipmentState::Upgrade);
}


void UOutfitWG::OnclickedWearing()
{
	if (player != nullptr)
	{
		bool result = player->InvenComp->WeaponSwitch(inventoryData.invenitem);
		if (result)
		{
			inventoryData.invenitem.WeaponData.isEquip = true;
		}
		WearingSwitch();
	}

}

void UOutfitWG::OnclickedOff()
{
	if (player != nullptr)
	{
		bool result = player->InvenComp->WeaponOff(inventoryData.invenitem);
		if (result)
		{
			inventoryData.invenitem.WeaponData.isEquip = false;
		}
		WearingSwitch();
	}
}

void UOutfitWG::OnclickedBack()
{
	RemoveFromParent();
}

void UOutfitWG::ButtonSwitch(bool isMaxLevel) // 강화 돌파 스위치
{
	if (isMaxLevel)
	{
		Button_Upgrade->SetVisibility(ESlateVisibility::Visible);
		Button_LevelUp->SetVisibility(ESlateVisibility::Hidden);
		ReinforceSwitch(EEquipmentState::Upgrade);
	}
	else
	{
		Button_LevelUp->SetVisibility(ESlateVisibility::Visible);
		Button_Upgrade->SetVisibility(ESlateVisibility::Hidden);
		ReinforceSwitch(EEquipmentState::LevelUp);
	}
}

void UOutfitWG::ReinforceSwitch(EEquipmentState state) //패널 스위치
{
	Panel_Reinforce->ClearChildren();
	switch (state)
	{
	case EEquipmentState::Detail:
		Panel_Detail->SetVisibility(ESlateVisibility::Visible);
		WearingSwitch();
		break;
	case EEquipmentState::Upgrade:
		Panel_Reinforce->AddChildToCanvas(UpgradeWG);
		Panel_Detail->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EEquipmentState::LevelUp:
		Panel_Reinforce->AddChildToCanvas(LevelUpWG);
		Panel_Detail->SetVisibility(ESlateVisibility::Hidden);
		break;
	}
}

void UOutfitWG::WearingSwitch()
{
	int32 value = player->InvenComp->CheckWeaponisEquip(); //착용하고 있는 아이템 체크
	if (value > -1) //있으면
	{
		if (player->InvenComp->invenItemArr[value] == inventoryData.invenitem) //나온것이 현재 장비창에 띄운 인포랑 같은지
		{
			//맞으면 해체를 활성화 시킨다.
			// 교체을 비활성화 시킨다.
			Button_Off->SetVisibility(ESlateVisibility::Visible);
			Button_Change->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			//아니면 교체을 활성화 시킨다.
			//해체를 비활성화 시킨다.
			TB_Wearing->SetText(FText::FromString(TEXT("교체")));
			Button_Change->SetVisibility(ESlateVisibility::Visible);
			Button_Off->SetVisibility(ESlateVisibility::Hidden);

		}
	}
	else
	{
		TB_Wearing->SetText(FText::FromString(TEXT("장착")));
		Button_Change->SetVisibility(ESlateVisibility::Visible);
		Button_Off->SetVisibility(ESlateVisibility::Hidden);
	}

	description->ReceiveSelectSlotData(inventoryData);

}


void UOutfitWG::ReceiveUseItem(FInvenItem ModifiedItem)
{
	inventoryData.invenitem = ModifiedItem;
	SendToInvenInfo.Broadcast(inventoryData);
	UpdateOutfitWG();
}

