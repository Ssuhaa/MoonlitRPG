// Fill out your copyright notice in the Description page of Project Settings.


#include "OutfitWG.h"
#include "ItemDescriptionWG.h"
#include <UMG/Public/Components/ScaleBox.h>
#include <UMG/Public/Components/Button.h>


void UOutfitWG::ButtonBinding()
{
	Button_Detail->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedDetail);
	Button_LevelUp->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedLevelUp);
	Button_Upgrade->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedUpgrade);
	Button_Wearing->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedWearing);
	Button_Back->OnPressed.AddDynamic(this, &UOutfitWG::OnclickedBack);
}

void UOutfitWG::OnclickedDetail()
{
	
}
void UOutfitWG::OnclickedLevelUp()
{

}

void UOutfitWG::OnclickedUpgrade()
{

}


void UOutfitWG::OnclickedWearing()
{

}

void UOutfitWG::OnclickedBack()
{
	RemoveFromParent();
}


void UOutfitWG::PopupOutfit(class UItemDescriptionWG* Description)
{
	itemDescription->AddChild(Description);
	AddToViewport();
}
