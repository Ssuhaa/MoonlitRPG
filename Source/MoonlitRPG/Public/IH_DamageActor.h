// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetActorBase.h"
#include "IH_DamageActor.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AIH_DamageActor : public AWidgetActorBase
{
	GENERATED_BODY()
	
public:
	AIH_DamageActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	float currentTime = 0;
};
