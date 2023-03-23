// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PuzzleGuide.generated.h"

UCLASS()
class MOONLITRPG_API APuzzleGuide : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APuzzleGuide();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FVector> Movepos;

	int32 posIndex = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class AAIController* ai;

	void MoveToPuzzleActor();
	void ReceivePuzzleArr(TArray<class UStaticMeshComponent*> arr);
	void MoveToPos(FVector pos);

	FVector originPos;

	bool giveHint = false;
	float currTime = 0;
};
