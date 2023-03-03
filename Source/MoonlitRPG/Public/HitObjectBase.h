// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitObjectBase.generated.h"

UCLASS()
class MOONLITRPG_API AHitObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* compSpawnPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AItemBase>> spawnItems;

	int32 currCount = 0;
	int32 hitCount;

public:
	void DropItem();
};
