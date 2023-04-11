// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "SequencerNPC.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API ASequencerNPC : public ACharacterBase
{
	GENERATED_BODY()

	ASequencerNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
