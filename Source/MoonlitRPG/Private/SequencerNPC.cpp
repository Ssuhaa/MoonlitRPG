// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerNPC.h"

ASequencerNPC::ASequencerNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeScale3D(FVector(1.5));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SetActorHiddenInGame(true);
}

void ASequencerNPC::BeginPlay()
{
	Super::BeginPlay();
}

void ASequencerNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}