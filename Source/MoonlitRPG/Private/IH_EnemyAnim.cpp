// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_EnemyAnim.h"
#include "IH_Enemy.h"

void UIH_EnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ownerPawn = TryGetPawnOwner();
	me=Cast<AIH_Enemy>(ownerPawn);
}

void UIH_EnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}