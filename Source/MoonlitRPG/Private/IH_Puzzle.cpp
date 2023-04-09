// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_Puzzle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "PuzzleGuide.h"
#include <Particles/ParticleSystemComponent.h>
#include <Kismet/GameplayStatics.h>
#include "IH_TreasureBox.h"

// Sets default values
AIH_Puzzle::AIH_Puzzle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(compRoot);

	compGuidePos = CreateDefaultSubobject<USceneComponent>(TEXT("Guide Pos Component"));
	compGuidePos->SetupAttachment(RootComponent);

	for (int32 i = 0; i < componentCount; i++)
	{
		FString meshName = FString::Printf(TEXT("Puzzle Mesh Component%d"), i);
		UStaticMeshComponent* currComp = CreateDefaultSubobject<UStaticMeshComponent>(*meshName);
		currComp->SetupAttachment(RootComponent);
		currComp->SetCanEverAffectNavigation(false);
		currComp->SetCollisionObjectType(ECC_GameTraceChannel1);
		currComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		currComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
		currComp->SetRelativeScale3D(FVector(0.1));
		meshArr.Add(currComp);
	}

	ConstructorHelpers::FClassFinder<APuzzleGuide>tempGuide(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/BP_PuzzleGuider.BP_PuzzleGuider_C'"));
	if (tempGuide.Succeeded())
	{
		guideFactory = tempGuide.Class;
	}

	ConstructorHelpers::FClassFinder<AInteractiveObjectBase>tempBox(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/BP_PuzzleTreasureBox.BP_PuzzleTreasureBox_C'"));
	if (tempBox.Succeeded())
	{
		treasureBoxFactory = tempBox.Class;
	}

	compRock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawn Rock Component"));
	compRock->SetupAttachment(RootComponent);
	compRock->SetRelativeScale3D(FVector(0.8, 0.5, 0.3));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempRock(TEXT("/Script/Engine.StaticMesh'/Game/Geometry/Meshes/SM_RockCube.SM_RockCube'"));
	if (tempRock.Succeeded())
	{
		compRock->SetStaticMesh(tempRock.Object);
	}

	compBoxPos = CreateDefaultSubobject<USceneComponent>(TEXT("Box Pos Component"));
	compBoxPos->SetupAttachment(compRock);
	compBoxPos->SetRelativeLocation(FVector(25, -40, 115));
	compBoxPos->SetRelativeRotation(FRotator(0, 90, 0));

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

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempLoot(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Loot_6.P_Loot_6'"));
	if (tempLoot.Succeeded())
	{
		particleArr.Add(tempLoot.Object);
	}
}

// Called when the game starts or when spawned
void AIH_Puzzle::BeginPlay()
{
	Super::BeginPlay();

	puzzleGuide = GetWorld()->SpawnActor<APuzzleGuide>(guideFactory, compGuidePos->GetComponentLocation(), compGuidePos->GetComponentRotation());
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

			FVector hitLoc = mesh->GetComponentLocation();
//			hitLoc.Z = 0;
			SpawnEffect(particleArr[2], hitLoc, FVector(0.3));
			SpawnEffect(particleArr[3], mesh->GetComponentLocation() + (mesh->GetUpVector() * 105), FVector(0.5));
//			UE_LOG(LogTemp, Warning, TEXT("Add Mesh : %s"), *mesh->GetName());
		}
	}
	else
	{
		hitMeshArr.Add(mesh);

		FVector hitLoc = mesh->GetComponentLocation();
//		hitLoc.Z = 0;
		SpawnEffect(particleArr[2], hitLoc, FVector(0.3));
		SpawnEffect(particleArr[3], mesh->GetComponentLocation() + (mesh->GetUpVector() * 105), FVector(0.5));
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
					lootArr[j]->DestroyComponent();
				}
				hitMeshArr.Empty();
				lootArr.Empty();
				UE_LOG(LogTemp, Warning, TEXT("Bad"));
			}
		}
	}
	
	if (hitMeshArr.IsValidIndex(meshArr.Num()-1))
	{
		if (meshArr[meshArr.Num() - 1] == hitMeshArr[hitMeshArr.Num() - 1])
		{
//			UE_LOG(LogTemp, Warning, TEXT("Correct!!!!!!!!!!!"));
			SpawnBox();
		}
	}
}

void AIH_Puzzle::SpawnEffect(class UParticleSystem* particle, FVector loc, FVector size)
{
	UParticleSystemComponent* compEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, loc);
	compEffect->SetRelativeScale3D(size);

	if (particle == particleArr[3])
	{
		lootArr.Add(compEffect);
	}
}

void AIH_Puzzle::SpawnBox()
{
	if (!isBoxSpawned)
	{
		AInteractiveObjectBase* spawnObject = GetWorld()->SpawnActor<AInteractiveObjectBase>(treasureBoxFactory, compBoxPos->GetComponentLocation(), compBoxPos->GetComponentRotation());
		SpawnEffect(particleArr[0], compBoxPos->GetComponentLocation(), FVector(0.5));
		SpawnEffect(particleArr[1], compBoxPos->GetComponentLocation(), FVector(0.8));
		isBoxSpawned = true;
		puzzleGuide->Destroy();
	}
}