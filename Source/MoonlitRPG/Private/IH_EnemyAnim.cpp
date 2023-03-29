// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_EnemyAnim.h"
#include "EnemyBase.h"
#include "SH_Player.h"

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

void UIH_EnemyAnim::AnimNotify_EnemyDieEnd()
{
	me->FSM->bDiedone = true;
}

void UIH_EnemyAnim::AnimNotify_EnemyAttack()
{
	if (me->FSM->target != nullptr)
	{
		if (me->FSM->PlayerCheck())
		{
			me->RandomHitImpact(me->FSM->target->GetActorLocation() - (me->FSM->target->GetActorUpVector() * 30));

			me->FSM->target->DamagedPlayer(int32(me->FSM->enemyDamage));
		}
	}
}

void UIH_EnemyAnim::AnimNotify_EnemyAttackEnd()
{
	me->FSM->bAttackEnd = true;
}