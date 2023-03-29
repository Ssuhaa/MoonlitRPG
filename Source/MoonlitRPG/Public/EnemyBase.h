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

	float dissolveValue = 1;
	float currTime = 0;

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInstanceDynamic*> instArr;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemy_FSM* FSM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* compEnemyHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UIH_EnemyHPUI* enemyHPUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf <class AItemBase>> spawnItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf <class AMoney>> spawnMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* itemSpawnPos;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* compExclamation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* hitImpact;

	UPROPERTY(EditAnywhere)
	TArray<UParticleSystem*> impactEffectArr;

	void SetActive(bool bActive);
	void ImpactEffect();
	void RandomHitImpact(FVector loc);

	int32 EnemyManagerIdx;

};
