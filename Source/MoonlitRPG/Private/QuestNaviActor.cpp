// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestNaviActor.h"
#include "QuestNaviWG.h"
#include <UMG/Public/Components/WidgetComponent.h>

AQuestNaviActor::AQuestNaviActor()
{
	ConstructorHelpers::FClassFinder<UQuestNaviWG> TempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_QuestNavi.WG_QuestNavi_C'"));
	if (TempWG.Succeeded())
	{
		NaviFactory = TempWG.Class;
	}
	NaviWG = CreateWidget<UQuestNaviWG>(GetWorld(),NaviFactory);
}

void AQuestNaviActor::BeginPlay()
{
	Super::BeginPlay();
	NaviWG->NaviActor = this;
	compWidget->SetWidget(NaviWG);
	compWidget->SetWidgetSpace(EWidgetSpace::Screen);
	compWidget->SetDrawAtDesiredSize(true);
}

void AQuestNaviActor::SetActiveNaviWG(bool value)
{

	compWidget->SetVisibility(value);

}
