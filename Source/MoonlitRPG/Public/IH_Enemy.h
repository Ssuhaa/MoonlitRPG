// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "IH_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AIH_Enemy : public ACharacterBase
{
	GENERATED_BODY()

public:
	AIH_Enemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemy_FSM* FSM;
};
