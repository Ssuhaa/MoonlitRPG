// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_EnemyAnim.h"
#include "EnemyBase.h"

void UIH_EnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ownerPawn = TryGetPawnOwner();
	me=Cast<AEnemyBase>(ownerPawn);
}

void UIH_EnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UIH_EnemyAnim::AnimNotify_DieEnd()
{
	me->FSM->bDiedone = true;
}