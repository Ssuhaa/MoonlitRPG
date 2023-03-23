// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IH_Puzzle.generated.h"

UCLASS()
class MOONLITRPG_API AIH_Puzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIH_Puzzle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* compRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* compBoxSpawnPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMesh1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMesh3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMesh4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UStaticMeshComponent*> meshArr;

	UPROPERTY(EditAnywhere)
	class APuzzleGuide* puzzleGuide;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APuzzleGuide> guideFactory;
};
