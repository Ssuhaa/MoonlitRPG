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

	TArray<UParticleSystemComponent*> lootArr;

	void CheckAnswer();
	void ActivePuzzle(class UParticleSystem* particle, FVector loc, FVector size);
	void SpawnBox();

	UFUNCTION()
	void FinishSequence();

	UPROPERTY()
	TArray<USoundBase*> activeSoundArr;

	UPROPERTY(EditAnywhere)
	class ASH_Player* player;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* compRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* compGuidePos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* compBoxPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compRock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> createMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<class UStaticMeshComponent*> meshArr;
	UPROPERTY(EditAnywhere, Category = "Puzzle")
	class APuzzleGuide* puzzleGuide;
	UPROPERTY(EditAnywhere, Category = "Puzzle")
	TSubclassOf<class APuzzleGuide> guideFactory;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TArray<class UStaticMeshComponent*> hitMeshArr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> meshPos;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AInteractiveObjectBase> treasureBoxFactory;
	UPROPERTY(EditAnywhere, Category = "Effect")
	TArray<UParticleSystem*> particleArr;

	UPROPERTY(EditAnywhere)
	int32 componentCount = 5;

	UPROPERTY(EditAnywhere)
	class ULevelSequence* treasureSequence;
	UPROPERTY(EditAnywhere)
	class ULevelSequencePlayer* sequencePlayer;

	int32 hitCount = 0;
	bool isBoxSpawned = false;

	void ReceiveMeshArr(class UStaticMeshComponent* mesh);
	void PlayActiveSound(FVector meshLoc);
};
