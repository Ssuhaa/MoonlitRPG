// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_PlayerAnim.h"
#include "SH_Player.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "AttackComponent.h"
#include "IH_DieUI.h"
#include "InventoryComponent.h"
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

		if (bChangePose && currWeapon != EWeaponType::None)
		{
			currentTime += DeltaSeconds;

			if (currentTime > 5)
			{
				currentTime = 0;
				if(Player->AttackComp->isAttacking) return;

				switch (currWeapon)
				{
					case EWeaponType::Dagger:
					{
						Player->PlayAnimMontage(Player->AttackComp->daggerMontage, 1.0f, FName(TEXT("Put_In")));
						UE_LOG(LogTemp, Warning, TEXT("Put In"));
						break;
					}
					case EWeaponType::Sword:
					{
						Player->PlayAnimMontage(Player->AttackComp->swordMontage, 1.0f, FName(TEXT("Put_In")));
						UE_LOG(LogTemp, Warning, TEXT("Put In"));
						break;
					}
				}
			}
		}
	}
}

void USH_PlayerAnim::SwitchCheck(FDamageRange handRange, FDamageRange daggerRange, FDamageRange swordRange)
{
	// EWeaponType currWeapon = Player->InvenComp->WhatKindOfEquipWeapon();

	switch (currWeapon)
	{
		case EWeaponType::None:
		{
			Player->AttackComp->TargetCheck(handRange);
			break;
		}
		case EWeaponType::Dagger:
		{
			Player->AttackComp->TargetCheck(daggerRange);
			break;
		}
		case EWeaponType::Sword:
		{
			Player->AttackComp->TargetCheck(swordRange);
			break;
		}
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
	SwitchCheck(Player->AttackComp->CommonRange, Player->AttackComp->DG_CommonRange, Player->AttackComp->GS_CommonRange);
}

void USH_PlayerAnim::AnimNotify_IntensiveAttack1()
{
	SwitchCheck(FDamageRange{}, Player->AttackComp->DG_IntensiveRange1, Player->AttackComp->GS_IntensiveRange1);	
}

void USH_PlayerAnim::AnimNotify_IntensiveAttack2()
{
	SwitchCheck(FDamageRange{}, Player->AttackComp->DG_IntensiveRange2, Player->AttackComp->GS_IntensiveRange2);
}

void USH_PlayerAnim::AnimNotify_SpecialAttack1()
{
	SwitchCheck(FDamageRange{}, Player->AttackComp->DG_SpecialRange1, Player->AttackComp->GS_SpecialRange1);
}

void USH_PlayerAnim::AnimNotify_SpecialAttack2()
{
	SwitchCheck(FDamageRange{}, Player->AttackComp->DG_SpecialRange2, Player->AttackComp->GS_SpecialRange2);
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

void USH_PlayerAnim::AnimNotify_Put_In_Start()
{
	bChangePose = false;
	Player->GetCharacterMovement()->DisableMovement();
}

void USH_PlayerAnim::AnimNotify_Put_In_End()
{
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
