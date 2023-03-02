// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObjectBase.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include "ItemBase.h"

// Sets default values
AInteractiveObjectBase::AInteractiveObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(compMesh);

	compSpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Position Component"));
	compSpawnPos->SetupAttachment(RootComponent);
	compSpawnPos->SetRelativeLocation(FVector(0, 0, 30));
}

// Called when the game starts or when spawned
void AInteractiveObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveObjectBase::DropItem()
{
	float randZ = FMath::RandRange(0, 360);
	compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));

	float randDist = FMath::RandRange(60, 100);
	FVector randForward = compSpawnPos->GetForwardVector()*randDist;
	compSpawnPos->SetRelativeLocation(compSpawnPos->GetComponentLocation()+randForward);

	if (spawnItems.IsValidIndex(0))
	{
		for (int32 i = 1; i <= spawnItems.Num(); i++)
		{
			int32 randNum = FMath::RandRange(0, spawnItems.Num()-1);
			GetWorld()->SpawnActor<AItemBase>(spawnItems[randNum], compSpawnPos->GetComponentLocation(), compSpawnPos->GetComponentRotation());
		}
	}
}