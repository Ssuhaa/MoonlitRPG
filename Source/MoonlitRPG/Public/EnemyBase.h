// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemy_FSM* FSM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* compEnemyHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UIH_EnemyHPUI* enemyHPUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* itemSpawnPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf <class AItemBase>> spawnItems;

public:
	void SetActive(bool bActive);
};
