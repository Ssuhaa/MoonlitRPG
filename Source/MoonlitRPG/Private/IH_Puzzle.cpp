// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_Puzzle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "PuzzleGuide.h"

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

// 	for (int32 i = 0; i < 4; i++)
// 	{
// 		FString str = FString::Printf(TEXT("Puzzle%d"), i);
// 		compMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(*str);
// 		compMesh1->SetupAttachment(RootComponent);
// 	}

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

	ConstructorHelpers::FClassFinder<AInteractiveObjectBase>tempBox(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/BP_TreasureBox.BP_TreasureBox_C'"));
	if (tempBox.Succeeded())
	{
		treasureBoxFactory = tempBox.Class;
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
	hitMeshArr.Add(mesh);
	mesh->SetMaterial(0, materialFactory[0]);
	UE_LOG(LogTemp, Warning, TEXT("Add Mesh : %s"), *mesh->GetName());
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
				}
				hitMeshArr.Empty();
				UE_LOG(LogTemp, Warning, TEXT("Bad"));
			}
		}
	}

	if (hitMeshArr.IsValidIndex(meshArr.Num()-1))
	{
		if (meshArr[meshArr.Num() - 1] == hitMeshArr[hitMeshArr.Num() - 1])
		{
			UE_LOG(LogTemp, Warning, TEXT("Correct!!!!!!!!!!!"));
			GetWorld()->SpawnActor<AInteractiveObjectBase>(treasureBoxFactory, compBoxSpawnPos->GetComponentTransform());
			puzzleGuide->Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("InCorrect!!!!!!!!!!!"));
		}
	}
}