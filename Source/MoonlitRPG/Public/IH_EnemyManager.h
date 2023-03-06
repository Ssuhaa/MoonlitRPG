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
	float createTime = 3;
	
	UPROPERTY(EditAnywhere, Category = DelayTime)
	float minTime = 3;

	UPROPERTY(EditAnywhere, Category = DelayTime)
	float maxTime = 10;
};
