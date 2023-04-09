// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_TreasureBox.h"
#include <Particles/ParticleSystemComponent.h>
#include "IH_InteractionUI.h"
#include <Kismet/GameplayStatics.h>

AIH_TreasureBox::AIH_TreasureBox()
{
	compTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Top Component"));
	compTop->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempOpen(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/GoodFXLevelUp/FX/Particles/PS_GFXLU_Galaxy_Open.PS_GFXLU_Galaxy_Open'"));
	if (tempOpen.Succeeded())
	{
		openEffect = tempOpen.Object;
	}
}

void AIH_TreasureBox::BeginPlay()
{
	Super::BeginPlay();

	topMat = UMaterialInstanceDynamic::Create(compTop->GetMaterial(0), nullptr);
	bottomMat = UMaterialInstanceDynamic::Create(compMesh->GetMaterial(0), nullptr);

	compTop->SetMaterial(0, topMat);
	compMesh->SetMaterial(0, bottomMat);
}

void AIH_TreasureBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isOpen)
	{
		compTop->SetRelativeRotation(FRotator(rotationY, 180, 0));
		rotationY += DeltaTime * 500;
		if (rotationY > 80)
		{
			rotationY=80;
			currTime += DeltaTime;
			if (currTime > 1.0f)
			{
				dissolveTop -= DeltaTime * 0.7;
				topMat->SetScalarParameterValue(TEXT("Disappear"), dissolveTop);
				if (currTime > 1.3f)
				{
					dissolveBottom -= DeltaTime * 0.7;
					bottomMat->SetScalarParameterValue(TEXT("Disappear"), dissolveBottom);
					if(currTime > 3.0f)
					{
						Destroy();
					}
				}
			}
		}
	}
}

// void AIH_TreasureBox::OpenBox()
// {
// 	if (spawnMoney.IsValidIndex(0))
// 	{
// 		int32 randCoin = FMath::RandRange(2, 3);
// 
// 		for (int32 i = 1; i <= randCoin; i++)
// 		{
// 			float randZ = FMath::RandRange(0, 360);
// 			compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));
// 
// 			GetWorld()->SpawnActor<AMoney>(spawnMoney[0], compSpawnPos->GetComponentLocation(), compSpawnPos->GetComponentRotation());
// 		}
// 	}
// 
// 	if (spawnItems.IsValidIndex(0))
// 	{
// 		int32 randAmount = FMath::RandRange(3, 4);		// °³¼ö ·£´ý»Ì±â
// 
// 		for (int32 i = 1; i <= randAmount; i++)
// 		{
// 			float randZ = FMath::RandRange(0, 360);
// 			compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));
// 
// 			int32 randIndex = FMath::RandRange(0, spawnItems.Num() - 1);		// ¹è¿­ ¿ä¼Ò ·£´ý»Ì±â
// 			GetWorld()->SpawnActor<AItemBase>(spawnItems[randIndex], compSpawnPos->GetComponentLocation(), compSpawnPos->GetComponentRotation());
// 		}
// 	}
// 
// 	if (interactionUI != nullptr)
// 	{
// 		interactionUI->RemoveFromParent();
// 	}
// 
// 	Destroy();
// }

void AIH_TreasureBox::Interaction()
{
	Super::Interaction();
	
	isOpen = true;
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	compTop->SetCollisionEnabled(ECollisionEnabled::NoCollision);

 	if (interactionUI != nullptr)
 	{
 		interactionUI->RemoveFromParent();
		interactionUI = nullptr;
 	}

	UParticleSystemComponent* currParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), openEffect, compMesh->GetComponentLocation() + compMesh->GetUpVector() * 60, compMesh->GetComponentRotation());
	currParticle -> SetRelativeScale3D(FVector(0.8));
}