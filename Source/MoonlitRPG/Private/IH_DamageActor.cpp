// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_DamageActor.h"
#include "IH_EnemyDamageUI.h"
#include <UMG/Public/Components/WidgetComponent.h>

AIH_DamageActor::AIH_DamageActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UIH_EnemyDamageUI>tempDamage(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_EnemyDamage.WG_EnemyDamage_C'"));
	if (tempDamage.Succeeded())
	{
		compWidget->SetWidgetClass(tempDamage.Class);
		compWidget->SetWidgetSpace(EWidgetSpace::Screen);
	}
}

void AIH_DamageActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIH_DamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;
	if (currentTime > 1.0)
	{
		Destroy();
	}
}

