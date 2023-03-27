// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IH_EnemyManager.generated.h"

UCLASS()
class MOONLITRPG_API AIH_EnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIH_EnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(EditAnywhere)
	TArray <class AEnemyBase*> enemyArr;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf <class AEnemyBase>> enemyFactory;

public:
	float currentTime;

	UPROPERTY(EditAnywhere, Category = DelayTime)
	float createTime;
	
	UPROPERTY(EditAnywhere, Category = DelayTime)
	float minTime = 10;

	UPROPERTY(EditAnywhere, Category = DelayTime)
	float maxTime = 15;

	UPROPERTY(EditAnywhere, Category = Spawn)
	int32 createNumber = 3;

	UPROPERTY(EditAnywhere, Category = Spawn)
	int32 spawnNumber = 5;

	UPROPERTY(EditAnywhere, Category = Spawn)
	int32 deathCount = 0;

	UPROPERTY(EditAnywhere, Category = Spawn)
	bool canSpawn = true;

	UPROPERTY(EditAnywhere, Category = Index)
	int32 EnemyManagerIdx;

	UPROPERTY()
	class ADataManager* DataManager;
};