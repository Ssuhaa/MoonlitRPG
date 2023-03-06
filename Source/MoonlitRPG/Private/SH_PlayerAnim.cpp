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
		//�̵� �ӵ� ��������
		FVector velocity = Player->GetVelocity();
		//���� ���� ��������
		FVector forwardVector = Player->GetActorForwardVector();
		//�� ���͸� ������ ���� dirV�� �Ҵ�.
		dirV = FVector::DotProduct(forwardVector, velocity);

		//�÷��̾��� �¿���� ��������
		FVector rightVector = Player->GetActorRightVector();
		//�� ���͸� ������ ���� dirH �Ҵ�
		dirH = FVector::DotProduct(rightVector, velocity);

		//���߿� �ִ��� ���� ����
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
