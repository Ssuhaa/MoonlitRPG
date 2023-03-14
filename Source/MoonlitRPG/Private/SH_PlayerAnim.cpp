// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_PlayerAnim.h"
#include "SH_Player.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "AttackComponent.h"
#include "IH_DieUI.h"

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
	Player->AttackComp->ResetAttack();
}

void USH_PlayerAnim::AnimNotify_NextCombo()
{
	Player->AttackComp->NextCombo();
}

void USH_PlayerAnim::AnimNotify_CommonAttack()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->CommonRange);
}

void USH_PlayerAnim::AnimNotify_IntensiveAttack1()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->IntensiveRange1);
}

void USH_PlayerAnim::AnimNotify_IntensiveAttack2()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->IntensiveRange2);
}

void USH_PlayerAnim::AnimNotify_SpecialAttack1()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->SpecialRange1);
}

void USH_PlayerAnim::AnimNotify_SpecialAttack2()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->SpecialRange2);
}

void USH_PlayerAnim::AnimNotify_DashEnd()
{
	Player->GetCharacterMovement()->DisableMovement();
}

void USH_PlayerAnim::AnimNotify_DashToWalk()
{
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void USH_PlayerAnim::AnimNotify_DamagedEnd()
{
	Player->AttackComp->ResetAttack();
}

void USH_PlayerAnim::AnimNotify_DieEnd()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	Player->dieUI->AddToViewport();
}

void USH_PlayerAnim::AnimNotify_GS_CommonAttack()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->GS_CommonRange);
}

void USH_PlayerAnim::AnimNotify_GS_IntensiveAttack1()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->GS_IntensiveRange1);
}

void USH_PlayerAnim::AnimNotify_GS_IntensiveAttack2()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->GS_IntensiveRange2);
}

void USH_PlayerAnim::AnimNotify_GS_SpecialAttack1()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->GS_SpecialRange1);
}

void USH_PlayerAnim::AnimNotify_GS_SpecialAttack2()
{
	Player->AttackComp->TargetCheck(Player->AttackComp->GS_SpecialRange2);
}