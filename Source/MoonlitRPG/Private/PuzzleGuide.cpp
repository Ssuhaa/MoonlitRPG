// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGuide.h"
#include <Components/CapsuleComponent.h>
#include <AIModule/Classes/AIController.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
APuzzleGuide::APuzzleGuide()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Geometry/Moonlit_character/SDbase_ChRigging.SDbase_ChRigging'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetRelativeScale3D(FVector(0.3));
		GetMesh()->SetRelativeLocation(FVector(0, 0, -45));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
	
	ConstructorHelpers::FClassFinder<UAnimInstance>tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrint/ABP_PuzzleGuide.ABP_PuzzleGuide_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel3);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
}

// Called when the game starts or when spawned
void APuzzleGuide::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDefaultController();
	ai = Cast<AAIController>(GetController());

	originPos=GetActorLocation();
}

// Called every frame
void APuzzleGuide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float period = 2.0f;
	float amplitude = 5.0f;

	float DeltaMove = amplitude * FMath::Sin(2*PI*GetWorld()->GetTimeSeconds() * 2/period);

	FVector currPos = GetActorLocation();
	FVector newPos = currPos + FVector(0.0f, 0.0f, DeltaMove);

	SetActorLocation(newPos);

	if (giveHint)
	{
		currTime += DeltaTime;
		if (currTime > 1)
		{
 			MoveToPos(Movepos[posIndex]);
		}
 	}
}

// Called to bind functionality to input
void APuzzleGuide::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APuzzleGuide::MoveToPuzzleActor()
{
	if (ai && Movepos.IsValidIndex(0))
	{
		giveHint = true;
	}
}

void APuzzleGuide::ReceivePuzzleArr(TArray<class UStaticMeshComponent*> arr)
{
	for (int32 i = 0; i < arr.Num(); i++)
	{
		Movepos.Add(arr[i]->GetComponentLocation());
	}
}

void APuzzleGuide::MoveToPos(FVector pos)
{
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		currTime = 0;
		posIndex++;
		UE_LOG(LogTemp,Warning,TEXT("%d"),posIndex);
		if (posIndex >= Movepos.Num())
		{
			ai->MoveToLocation(originPos);
			giveHint = false;
			posIndex = 0;
		}
	}
}