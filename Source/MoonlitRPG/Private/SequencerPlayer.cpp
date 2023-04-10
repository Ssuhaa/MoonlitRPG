// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerPlayer.h"

// Sets default values
ASequencerPlayer::ASequencerPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Animation/Meshes/SK_SdCharacter.SK_SdCharacter'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeScale3D(FVector(1.5));
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

	SetActorHiddenInGame(true);
}

// Called when the game starts or when spawned
void ASequencerPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASequencerPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}