// Fill out your copyright notice in the Description page of Project Settings.


#include "HpFullPopup.h"
#include <UMG/Public/Components/Button.h>


void UHpFullPopup::ButtonBinding()
{
	Button_Cancel->OnPressed.AddDynamic(this, &UHpFullPopup::OnClickedCancel);
	Button_Close->OnPressed.AddDynamic(this, &UHpFullPopup::OnClickedClose);
}

void UHpFullPopup::OnClickedCancel()
{
	RemoveFromParent();
}

void UHpFullPopup::OnClickedClose()
{
	RemoveFromParent();
}