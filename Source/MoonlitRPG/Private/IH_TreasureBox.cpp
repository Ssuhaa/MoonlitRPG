// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_TreasureBox.h"
#include <Particles/ParticleSystemComponent.h>
#include "IH_InteractionUI.h"

AIH_TreasureBox::AIH_TreasureBox()
{
	spawnEffect_1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Spawn Effect_1"));
	spawnEffect_1->SetupAttachment(RootComponent);
	spawnEffect_1->SetActive(false);

	spawnEffect_2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Spawn Effect_2"));
	spawnEffect_2->SetupAttachment(RootComponent);
	spawnEffect_2->SetActive(false);

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempSpawn(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Confetti.P_Confetti'"));
	if (tempSpawn.Succeeded())
	{
		particleArr.Add(tempSpawn.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempSpawn2(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/SimpleCartoonFX/FX/Land_Smoke.Land_Smoke'"));
	if (tempSpawn2.Succeeded())
	{
		particleArr.Add(tempSpawn2.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempSpawn3(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/GoodFXLevelUp/FX/Particles/PS_GFXLU_Holy.PS_GFXLU_Holy'"));
	if (tempSpawn3.Succeeded())
	{
		particleArr.Add(tempSpawn3.Object);
	}
}

void AIH_TreasureBox::BeginPlay()
{
	Super::BeginPlay();

	spawnEffect_1->SetTemplate(particleArr[2]);
	spawnEffect_1->SetActive(true);
	spawnEffect_2->SetTemplate(particleArr[1]);
	spawnEffect_2->SetActive(true);
}

void AIH_TreasureBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIH_TreasureBox::Interaction()
{
// 	spawnEffect_1->SetRelativeScale3D(FVector(0.5, 0.5, 0.2));
// 	spawnEffect_1->SetTemplate(particleArr[2]);
// 	spawnEffect_1->SetActive(true);

	Super::Interaction();
}