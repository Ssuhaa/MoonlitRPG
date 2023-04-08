// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "SequencerPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API ASequencerPlayer : public ACharacterBase
{
	GENERATED_BODY()
	
	ASequencerPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
