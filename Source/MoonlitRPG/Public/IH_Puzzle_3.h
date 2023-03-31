// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IH_Puzzle.h"
#include "IH_Puzzle_3.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AIH_Puzzle_3 : public AIH_Puzzle
{
	GENERATED_BODY()
	
	AIH_Puzzle_3();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
