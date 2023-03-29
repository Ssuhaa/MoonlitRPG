// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetSlotBase.h"
#include <Engine/Texture2D.h>
#include <UMG/Public/Components/Image.h>
#include <Kismet/GameplayStatics.h>
#include "DataManager.h"
#include "SH_Player.h"

UWidgetSlotBase::UWidgetSlotBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
 	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
}

void UWidgetSlotBase::NativeConstruct()
{
	Super::NativeConstruct();
	ButtonBinding();
}

void UWidgetSlotBase::ButtonBinding()
{

}

void UWidgetSlotBase::SlotClicked()
{

}

void UWidgetSlotBase::UpdateSlot(FInvenItem invenitem)
{
	invenData = DataManager->GetData(invenitem);
	ItemImage->SetBrushFromTexture(invenData.iteminfo.itemImage);
	SlotBG->SetBrushFromTexture(invenData.itemGradeData.Slotlmage, true);
}

