// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_Puzzle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "PuzzleGuide.h"
#include <Particles/ParticleSystemComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AIH_Puzzle::AIH_Puzzle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(compRoot);

	compBoxSpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("BoxSpawnPos Component"));
	compBoxSpawnPos->SetupAttachment(RootComponent);
	compBoxSpawnPos->SetRelativeLocation(FVector(-50, 0, 0));

	compMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Puzzle Mesh1"));
	compMesh1->SetupAttachment(RootComponent);
	meshArr.Add(compMesh1);

	compMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Puzzle Mesh2"));
	compMesh2->SetupAttachment(RootComponent);
	meshArr.Add(compMesh2);

	compMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Puzzle Mesh3"));
	compMesh3->SetupAttachment(RootComponent);
	meshArr.Add(compMesh3);

	compMesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Puzzle Mesh4"));
	compMesh4->SetupAttachment(RootComponent);
	meshArr.Add(compMesh4);

	ConstructorHelpers::FClassFinder<APuzzleGuide>tempGuide(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/BP_PuzzleGuider.BP_PuzzleGuider_C'"));
	if (tempGuide.Succeeded())
	{
		guideFactory = tempGuide.Class;
	}

	ConstructorHelpers::FClassFinder<AInteractiveObjectBase>tempBox(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/BP_Treasure.BP_Treasure_C'"));
	if (tempBox.Succeeded())
	{
		treasureBoxFactory = tempBox.Class;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempSpawn1(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/GoodFXLevelUp/FX/Particles/PS_GFXLU_Lightning.PS_GFXLU_Lightning'"));
	if (tempSpawn1.Succeeded())
	{
		particleArr.Add(tempSpawn1.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempSpawn2(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/SimpleCartoonFX/FX/Land_Smoke.Land_Smoke'"));
	if (tempSpawn2.Succeeded())
	{
		particleArr.Add(tempSpawn2.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/GoodFXLevelUp/FX/Particles/PS_GFXLU_Holy.PS_GFXLU_Holy'"));
	if (tempEffect.Succeeded())
	{
		particleArr.Add(tempEffect.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempLoot(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Loot_1.P_Loot_1'"));
	if (tempLoot.Succeeded())
	{
		particleArr.Add(tempLoot.Object);
	}
}

// Called when the game starts or when spawned
void AIH_Puzzle::BeginPlay()
{
	Super::BeginPlay();

	puzzleGuide = GetWorld()->SpawnActor<APuzzleGuide>(guideFactory, compRoot->GetComponentLocation(), compRoot->GetComponentRotation());
	puzzleGuide->ReceivePuzzleArr(meshArr);
}

// Called every frame
void AIH_Puzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIH_Puzzle::ReceiveMeshArr(class UStaticMeshComponent* mesh)
{
	bool isDifferent = true;

	if (hitMeshArr.IsValidIndex(0))
	{
		for (int32 i = 0; i < hitMeshArr.Num(); i++)
		{
			if (hitMeshArr[i] == mesh)
			{
				isDifferent = false;
				break;
			}
		}
		if (isDifferent)
		{
			hitMeshArr.Add(mesh);
			mesh->SetMaterial(0, materialFactory[0]);

			FVector hitLoc = mesh->GetComponentLocation();
			hitLoc.Z = 0;
			SpawnEffect(particleArr[2], hitLoc, FVector(0.3));
//			SpawnEffect(particleArr[3], mesh->GetComponentLocation(), FVector(0.5));
//			UE_LOG(LogTemp, Warning, TEXT("Add Mesh : %s"), *mesh->GetName());
		}
	}
	else
	{
		hitMeshArr.Add(mesh);
		mesh->SetMaterial(0, materialFactory[0]);

		FVector hitLoc = mesh->GetComponentLocation();
		hitLoc.Z = 0;
		SpawnEffect(particleArr[2], hitLoc, FVector(0.3));
//		SpawnEffect(particleArr[3], mesh->GetComponentLocation(), FVector(0.5));
//		UE_LOG(LogTemp, Warning, TEXT("Add Mesh : %s"), *mesh->GetName());
	}

	if (hitMeshArr.Num() == meshArr.Num())
	{
		CheckAnswer();
	}
}

void AIH_Puzzle::CheckAnswer()
{
	if (hitMeshArr.IsValidIndex(0))
	{
		for (int32 i = 0; i < hitMeshArr.Num(); i++)
		{
			if (hitMeshArr[i] == meshArr[i])
			{
				hitMeshArr[i] = meshArr[i];
				UE_LOG(LogTemp, Warning, TEXT("Good"));
			}
			else
			{
				for (int32 j = 0; j < hitMeshArr.Num(); j++)
				{
					hitMeshArr[j]->SetMaterial(0, materialFactory[1]);
					//lootArr[j]->Deactivate();
				}
				hitMeshArr.Empty();
				//lootArr.Empty();
				UE_LOG(LogTemp, Warning, TEXT("Bad"));
			}
		}
	}

	if (hitMeshArr.IsValidIndex(meshArr.Num()-1))
	{
		if (meshArr[meshArr.Num() - 1] == hitMeshArr[hitMeshArr.Num() - 1])
		{
//			UE_LOG(LogTemp, Warning, TEXT("Correct!!!!!!!!!!!"));
			GetWorld()->SpawnActor<AInteractiveObjectBase>(treasureBoxFactory, compBoxSpawnPos->GetComponentTransform());
			SpawnEffect(particleArr[0], compBoxSpawnPos->GetComponentLocation(), FVector(0.5));
			isBoxSpawned = true;
			puzzleGuide->Destroy();
		}
	}
}

void AIH_Puzzle::SpawnEffect(class UParticleSystem* particle, FVector loc, FVector size)
{
	UParticleSystemComponent* effect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, loc);
	effect->SetRelativeScale3D(size);

// 	if (particle == particleArr[2])
// 	{
// 		lootArr.Add(effect);
// 	}
}