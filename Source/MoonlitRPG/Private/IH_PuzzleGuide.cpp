// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_PuzzleGuide.h"
#include "Kismet/GameplayStatics.h"
#include <AIController.h>
#include "IH_Puzzle.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/PawnMovementComponent.h>

// Sets default values
AIH_PuzzleGuide::AIH_PuzzleGuide()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	SetRootComponent(compCapsule);
	compCapsule->SetCollisionObjectType(ECC_GameTraceChannel3);

	guideMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Guide"));
	guideMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Geometry/Moonlit_character/SDbase_ChRigging.SDbase_ChRigging'"));
	if (tempMesh.Succeeded())
	{
		guideMesh->SetSkeletalMesh(tempMesh.Object);
		guideMesh->SetRelativeScale3D(FVector(0.5));
		//guideMesh->SetRelativeLocation(FVector(0, 0, -90));
	}

	CharacterMovent = CreateDefaultSubobject<UPawnMovementComponent>(TEXT("Movement"));

}

// Called when the game starts or when spawned
void AIH_PuzzleGuide::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDefaultController();
	ai = Cast<AAIController>(GetController());
}

// Called every frame
void AIH_PuzzleGuide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AIH_PuzzleGuide::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AIH_PuzzleGuide::MoveToPuzzleActor()
{
	if (ai && Movepos.IsValidIndex(0))
	{
		ai->MoveToLocation(Movepos[0]);
		UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), Movepos[0].X, Movepos[0].Y, Movepos[0].Z);
	}
}

void AIH_PuzzleGuide::ReceivePuzzleArr(TArray<class UStaticMeshComponent*> arr)
{
	for (int32 i=0;i<arr.Num();i++)
	{
		Movepos.Add(arr[i]->GetComponentLocation());
	}
}