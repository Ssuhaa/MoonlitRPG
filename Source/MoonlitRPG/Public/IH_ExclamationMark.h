// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetActorBase.h"
#include "IH_ExclamationMark.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AIH_ExclamationMark : public AWidgetActorBase
{
	GENERATED_BODY()
	
public:
	AIH_ExclamationMark();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
