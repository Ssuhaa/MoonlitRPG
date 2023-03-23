// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetSlotBase.h"
#include <Engine/Texture2D.h>
#include <UMG/Public/Components/Image.h>

UWidgetSlotBase::UWidgetSlotBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Slot_Common.T_Slot_Common'"));
	if (tempT.Succeeded())
	{
		BGarray.Add(tempT.Object); //0번 Common
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT1(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Slot_Rare.T_Slot_Rare'"));
	if (tempT1.Succeeded())
	{
		BGarray.Add(tempT1.Object);	//1번 Rare
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT2(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Slot_Uinque.T_Slot_Uinque'"));
	if (tempT2.Succeeded())
	{
		BGarray.Add(tempT2.Object); //2번 Uinque
	}
	ConstructorHelpers::FObjectFinder <UTexture2D> tempT3(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Slot_Legendary.T_Slot_Legendary'"));
	if (tempT3.Succeeded())
	{
		BGarray.Add(tempT3.Object);	//3번 Legendary
	}
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

void UWidgetSlotBase::UpdateSlot(FInvenItem invenData)
{
	invenInfo = invenData;
	ItemImage->SetBrushFromTexture(invenInfo.iteminfomation.itemImage);
	int32 BGindex = int32(invenInfo.iteminfomation.itemgrade);
	SlotBG->SetBrushFromTexture(BGarray[BGindex], true);
}

void UWidgetSlotBase::UpdateSlot(FIteminfo iteminfo)
{
	ItemImage->SetBrushFromTexture(iteminfo.itemImage);
	int32 BGindex = int32(iteminfo.itemgrade);
	SlotBG->SetBrushFromTexture(BGarray[BGindex], true);
}