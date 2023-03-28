// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjectBase.h"
#include "IH_TreasureBox.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AIH_TreasureBox : public AInteractiveObjectBase
{
	GENERATED_BODY()
	
	AIH_TreasureBox();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

// 	UPROPERTY(EditAnywhere)
// 	class UParticleSystemComponent* spawnEffect_1;
// 
// 	UPROPERTY(EditAnywhere)
// 	class UParticleSystemComponent* spawnEffect_2;
// 
// 	UPROPERTY(EditAnywhere)
// 	TArray<UParticleSystem*> particleArr;

public:
	virtual void Interaction() override;
};
