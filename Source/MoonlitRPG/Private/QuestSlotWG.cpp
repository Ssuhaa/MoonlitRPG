// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSlotWG.h"

void UQuestSlotWG::UpdateSlot(FInvenItem* invenitem)
{
	Super::UpdateSlot(invenitem);
	TB_Amont->SetText(FText::AsNumber(invenitem->itemAmount));
}
