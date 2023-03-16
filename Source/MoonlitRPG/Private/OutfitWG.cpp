// Fill out your copyright notice in the Description page of Project Settings.


#include "OutfitWG.h"
#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/ScaleBox.h>
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/TextBlock.h>
#include "InventorySlotWG.h"
#include <UMG/Public/Components/Border.h>
#include <UMG/Public/Components/CanvasPanel.h>
#include "WeaponUpgradeWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "WeaponLevelUpWG.h"
#include "PreviewActor.h"
#include "InventoryWG.h"

UOutfitWG::UOutfitWG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder <UItemDescriptionWG> tempDesrip(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_ItemDescription.BP_ItemDescription_C'"));
	if (tempDesrip.Succeeded())
	{
		WGFactory.Add(tempDesrip.Class);
	}
	ConstructorHelpers::FClassFinder <UWeaponUpgradeWG> tempUpgrade(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_WeaponUpgrade.WG_WeaponUpgrade_C'"));
	if (tempUpgrade.Succeeded())
	{
		WGFactory.Add(tempUpgrade.Class);
	}
	ConstructorHelpers::FClassFinder <UWeaponLevelUpWG> tempLevelup(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_WeaponLevelUp.WG_WeaponLevelUp_C'"));
	if (tempLevelup.Succeeded())
	{
		WGFactory.Add(tempLevelup.Class);
	}
	description = CreateWidget<UItemDescriptionWG>(GetWorld(), WGFactory[0]);
	UpgradeWG = CreateWidget<UWeaponUpgradeWG>(GetWorld(), WGFactory[1]);
	LevelUpWG = CreateWidget<UWeaponLevelUpWG>(GetWorld(), WGFactory[2]);
}

void UOutfitWG::NativeConstruct()
{
	Super::NativeConstruct();
	ButtonBinding();
	
	OutfitActor = GetWorld()->SpawnActor<APreviewActor>(APreviewActor::StaticClass(), FVector(0, 0, 10000), FRotator(0));
	OutfitActor->SetPreviewMesh(SelectedSlot->invenInfo.weaponinfomaiton.Mesh);

	ReinforceSwitch(EEquipmentState::Detail);
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

	SendToInvenInfo.AddUObject(description, &UItemDescriptionWG::ReceiveSelectSlotitemData);
	SendToInvenInfo.AddUObject(UpgradeWG, &UWeaponUpgradeWG::SetUpGradeWG);
	SendToInvenInfo.AddUObject(LevelUpWG, &UWeaponLevelUpWG::ReceiveSelectSlotData);


	SendToInvenInfo.Broadcast(SelectedSlot->invenInfo);

	Button_Detail->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedDetail);
	Button_LevelUp->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedLevelUp);
	Button_Upgrade->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedUpgrade);
	Button_Wearing->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedWearing);
	Button_Back->OnPressed.AddUniqueDynamic(this, &UOutfitWG::OnclickedBack);
	UpgradeWG->Button_Upgrade->OnPressed.AddUniqueDynamic(this, &UOutfitWG::UpdateMoney);
}


void UOutfitWG::ReceiveSelectSlotData(class UInventorySlotWG* SelectSlot)
{
	SelectedSlot = SelectSlot;
	SelectedSlot->invenInfo.weaponinfomaiton.SendLevelUpClear.AddUObject(this, &UOutfitWG::UpdateOutfitWG);
	UpdateOutfitWG();
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
		player->InvenComp->WeaponSwitch(SelectedSlot->invenInfo);
	}
	WearingSwitch(SelectedSlot->invenInfo.weaponinfomaiton.isEquip);
}

void UOutfitWG::OnclickedBack()
{
	RemoveFromParent();
}

void UOutfitWG::UpdateOutfitWG()
{
	itemDescription->AddChild(description);
	UpdateMoney();

	int32 Typeindex = int32(SelectedSlot->invenInfo.weaponinfomaiton.WeaponType);
	FText weaponType;
	switch (Typeindex)
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

	FText itemname = FText::FromString(SelectedSlot->invenInfo.iteminfomation.ItemName);
	Text_WeaponName->SetText(itemname);


	SelectedSlot->invenInfo.weaponinfomaiton.Level == SelectedSlot->invenInfo.weaponinfomaiton.MaxLevel ? ButtonSwitch(true) : ButtonSwitch(false);
	
}

void UOutfitWG::WearingSwitch(bool isEquip)
{
	if (isEquip)
	{
		EquipPop->SetVisibility(ESlateVisibility::Visible);
		Button_Wearing->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		EquipPop->SetVisibility(ESlateVisibility::Hidden);
		Button_Wearing->SetVisibility(ESlateVisibility::Visible);
	}
}

void UOutfitWG::ButtonSwitch(bool isMaxLevel)
{
	if (isMaxLevel)
	{
		Button_Upgrade->SetVisibility(ESlateVisibility::Visible);
		Button_LevelUp->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Button_LevelUp->SetVisibility(ESlateVisibility::Visible);
		Button_Upgrade->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOutfitWG::ReinforceSwitch(EEquipmentState state) //강화 돌파 스위치
{
	OutfitState = state;
	Panel_Reinforce->ClearChildren();
	switch (OutfitState)
	{
	case EEquipmentState::Detail:
		SelectedSlot->invenInfo.weaponinfomaiton.isEquip ? WearingSwitch(true) : WearingSwitch(false);
		Panel_Detail->SetVisibility(ESlateVisibility::Visible);
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

void UOutfitWG::UpdateMoney()
{
	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	if (player != nullptr)
	{
		Text_Money->SetText(FText::AsNumber(player->InvenComp->Money));
	}
	SelectedSlot->invenInfo.weaponinfomaiton.Level == SelectedSlot->invenInfo.weaponinfomaiton.MaxLevel ? ButtonSwitch(true) : ButtonSwitch(false);
	ReinforceSwitch(EEquipmentState::LevelUp);
}

