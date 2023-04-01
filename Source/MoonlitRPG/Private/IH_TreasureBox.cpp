// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_TreasureBox.h"
#include <Particles/ParticleSystemComponent.h>
#include "IH_InteractionUI.h"

AIH_TreasureBox::AIH_TreasureBox()
{
	compTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Top Component"));
	compTop->SetupAttachment(RootComponent);
}

void AIH_TreasureBox::BeginPlay()
{
	Super::BeginPlay();

}

void AIH_TreasureBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isOpen)
	{
		compTop->SetRelativeRotation(FRotator(rotationY, 180, 0));
		rotationY += DeltaTime * 300;
		if (rotationY > 80)
		{
			rotationY=80;
			OpenBox();
		}
	}
}

void AIH_TreasureBox::OpenBox()
{
	if (spawnMoney.IsValidIndex(0))
	{
		int32 randCoin = FMath::RandRange(2, 3);

		for (int32 i = 1; i <= randCoin; i++)
		{
			float randZ = FMath::RandRange(0, 360);
			compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));

			GetWorld()->SpawnActor<AMoney>(spawnMoney[0], compSpawnPos->GetComponentLocation(), compSpawnPos->GetComponentRotation());
		}
	}

	if (spawnItems.IsValidIndex(0))
	{
		int32 randAmount = FMath::RandRange(3, 4);		// °³¼ö ·£´ý»Ì±â

		for (int32 i = 1; i <= randAmount; i++)
		{
			float randZ = FMath::RandRange(0, 360);
			compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));

			int32 randIndex = FMath::RandRange(0, spawnItems.Num() - 1);		// ¹è¿­ ¿ä¼Ò ·£´ý»Ì±â
			GetWorld()->SpawnActor<AItemBase>(spawnItems[randIndex], compSpawnPos->GetComponentLocation(), compSpawnPos->GetComponentRotation());
		}
	}

	if (interactionUI != nullptr)
	{
		interactionUI->RemoveFromParent();
	}

	Destroy();
}

void AIH_TreasureBox::Interaction()
{
	isOpen = true;
}