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

	for (int32 i = 0; i < createNumber; i++)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		float Random = FMath::FRandRange(0.0 , 1.0);

		AEnemyBase* enemy;
		if (Random < 0.2)
		{
			enemy= GetWorld()->SpawnActor<AEnemyBase>(enemyFactory[2], GetActorTransform(), param);
		}
		else if(Random >= 0.2 && Random <= 0.5)
		{
			enemy = GetWorld()->SpawnActor<AEnemyBase>(enemyFactory[1], GetActorTransform(), param);
		}
		else
		{
			enemy = GetWorld()->SpawnActor<AEnemyBase>(enemyFactory[0], GetActorTransform(), param);
		}
		enemy->Manager = this;
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

	if (canSpawn)
	{
		currentTime += DeltaTime;

		if (currentTime > createTime)
		{
			for (int32 i = 0; i < spawnNumber ; i++)
			{
				float randZ = FMath::RandRange(0, 360);
				SetActorRotation(FRotator(0, randZ, 0));

				float randDist = FMath::RandRange(300, 700);
				FVector randPos = GetActorForwardVector()*randDist;
				FVector upPos = GetActorUpVector()*90;

				enemyArr[0]->SetActorLocation(GetActorLocation()+randPos+upPos);
				enemyArr[0]->SetActorRotation(GetActorRotation());
				enemyArr[0]->SetActive(true);

// 				if (i == spawnNumber-1)
// 				{
// 					canSpawn = false;
// 				}

				enemyArr.RemoveAt(0);

// 				if (!enemyArr.IsValidIndex(0))
// 				{
// 					FActorSpawnParameters param;
// 					param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// 
// 					int32 randNum = FMath::RandRange(0, enemyFactory.Num() - 1);
// 					AEnemyBase* enemy = GetWorld()->SpawnActor<AEnemyBase>(enemyFactory[randNum], GetActorTransform(), param);
// 					enemy->Manager = this;
// 					enemy->SetActive(false);
// 
// 					enemyArr.Add(enemy);
// 				}

				createTime = FMath::RandRange(minTime, maxTime);
			}
			currentTime = 0;
			canSpawn = false;
		}
	}
}

