// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_EnemyManager.h"
#include "EnemyBase.h"
#include <UMG/Public/Components/WidgetComponent.h>

// Sets default values
AIH_EnemyManager::AIH_EnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIH_EnemyManager::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < 10; i++)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		int32 randNum = FMath::RandRange(0, enemyFactory.Num()-1);

		AEnemyBase* enemy = GetWorld()->SpawnActor<AEnemyBase>(enemyFactory[randNum], GetActorTransform(), param);
		enemy->SetActive(false);

		enemyArr.Add(enemy);
	}

	createTime = FMath::RandRange(minTime, maxTime);
}

// Called every frame
void AIH_EnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (enemyArr.Num() == 0) return;

	currentTime += DeltaTime;
	
	if (currentTime > createTime)
	{
		float randZ = FMath::RandRange(0, 360);
		SetActorRotation(FRotator(0, randZ, 0));

		float randDist = FMath::RandRange(300, 700);
		FVector randPos = GetActorForwardVector()*randDist;
		FVector upPos = GetActorUpVector()*90;

		enemyArr[0]->SetActorLocation(GetActorLocation()+randPos+upPos);
		enemyArr[0]->SetActorRotation(GetActorRotation());
		enemyArr[0]->SetActive(true);
		enemyArr.RemoveAt(0);

		currentTime = 0;

		createTime = FMath::RandRange(minTime, maxTime);
	}
}

