// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_Enemy.h"
#include "Enemy_FSM.h"

AIH_Enemy::AIH_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	FSM = CreateDefaultSubobject<UEnemy_FSM>(TEXT("FSM"));
}

void AIH_Enemy::BeginPlay()
{
	Super::BeginPlay();
}

void AIH_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}