// Fill out your copyright notice in the Description page of Project Settings.


#include "HitObjectBase.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include "ItemBase.h"
#include <Particles/ParticleSystemComponent.h>

// Sets default values
AHitObjectBase::AHitObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(compMesh);
	compMesh->SetCollisionObjectType(ECC_GameTraceChannel1);

	compSpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Position Component"));
	compSpawnPos->SetupAttachment(RootComponent);
	compSpawnPos->SetRelativeLocation(FVector(0, 0, 30));
}

// Called when the game starts or when spawned
void AHitObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	originSpawnPos = compSpawnPos->GetComponentLocation();
}

// Called every frame
void AHitObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitObjectBase::DropItem()
{
	float randZ = FMath::RandRange(0, 360);
	compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));

	float randDist = FMath::RandRange(50, 100);
	FVector randForward = compSpawnPos->GetForwardVector() * randDist;
	compSpawnPos->SetRelativeLocation(originSpawnPos + randForward);

	if (spawnItems.IsValidIndex(0))
	{
		if (currCount <= hitCount)
		{
			currCount += 1;
			int32 randNum = FMath::RandRange(0, spawnItems.Num() - 1);
			GetWorld()->SpawnActor<AItemBase>(spawnItems[randNum], compSpawnPos->GetRelativeLocation(), compSpawnPos->GetRelativeRotation());
		}
	}
}