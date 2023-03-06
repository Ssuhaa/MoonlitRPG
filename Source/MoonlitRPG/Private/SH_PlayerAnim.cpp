// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_PlayerAnim.h"
#include "SH_Player.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "AttackComponent.h"

void USH_PlayerAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	APawn* pawn = TryGetPawnOwner();
	Player =  Cast<ASH_Player>(pawn);
}

void USH_PlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player != nullptr)
	{
		//이동 속도 가져오기
		FVector velocity = Player->GetVelocity();
		//전방 벡터 가져오기
		FVector forwardVector = Player->GetActorForwardVector();
		//두 벡터를 내적한 값을 dirV에 할당.
		dirV = FVector::DotProduct(forwardVector, velocity);

		//플레이어의 좌우방향 가져오기
		FVector rightVector = Player->GetActorRightVector();
		//두 벡터를 내적한 값을 dirH 할당
		dirH = FVector::DotProduct(rightVector, velocity);

		//공중에 있는지 여부 세팅
		bAir = Player->GetCharacterMovement()->IsFalling();
	}
}

void USH_PlayerAnim::AnimNotify_AttackEnd()
{
	Player->AttackComp->isAttacking = false;
	Player->AttackComp->attackCount = 0;
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void USH_PlayerAnim::AnimNotify_NextCombo()
{
	Player->AttackComp->NextCombo();
}

void USH_PlayerAnim::AnimNotify_DashEnd()
{
	Player->GetCharacterMovement()->DisableMovement();
}

void USH_PlayerAnim::AnimNotify_DashToWalk()
{
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
