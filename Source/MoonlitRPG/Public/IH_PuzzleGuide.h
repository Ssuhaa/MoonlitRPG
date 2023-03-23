// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "IH_PuzzleGuide.generated.h"

UCLASS()
class MOONLITRPG_API AIH_PuzzleGuide : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AIH_PuzzleGuide();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	TArray<FVector> Movepos;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* compCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* guideMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPawnMovementComponent* CharacterMovent;

	UPROPERTY()
	class AAIController* ai;

	void MoveToPuzzleActor();
	void ReceivePuzzleArr(TArray<class UStaticMeshComponent*> arr);
};
