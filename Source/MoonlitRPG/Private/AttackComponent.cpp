// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy_FSM.h"
#include "EnemyBase.h"
#include "SH_Player.h"
#include <Kismet/KismetMathLibrary.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "HitObjectBase.h"
#include "SH_PlayerAnim.h"
#include "PlayerMainWG.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Mouse_LeftClick.Mouse_LeftClick'"));
	if (tempAction.Succeeded())
	{
		inputarray.Add(tempAction.Object);  //0�� ���콺 Ŭ��
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction1(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_E.Key_E'"));
	if (tempAction1.Succeeded())
	{
		inputarray.Add(tempAction1.Object);  //1�� EŰ
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction2(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_Q.Key_Q'"));
	if (tempAction2.Succeeded())
	{
		inputarray.Add(tempAction2.Object);  //2�� QŰ
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempAttackMont(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Animations/Montage/AMT_Player.AMT_Player'"));
	if (tempAttackMont.Succeeded())
	{
		attackMontage = tempAttackMont.Object;
	}
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<ASH_Player>(GetOwner());
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (coolTimeRunning)
	{
		intensiveDelay -= DeltaTime;
		if (intensiveDelay <= 0)
		{
			intensiveDelay = 0;
			coolTimeRunning = false;
		}
		player->MainHUD->UpdateEtime(intensiveDelay);

	}
}

void UAttackComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent)
{
	if (inputarray.IsValidIndex(0))
	{
		EnhancedInputComponent->BindAction(inputarray[0], ETriggerEvent::Triggered, this, &UAttackComponent::CommonAttack); // Ŭ��
		EnhancedInputComponent->BindAction(inputarray[1], ETriggerEvent::Triggered, this, &UAttackComponent::intensiveAttack); // E
		EnhancedInputComponent->BindAction(inputarray[2], ETriggerEvent::Triggered, this, &UAttackComponent::SpecialAttack); // Q
	}
}

void UAttackComponent::NextCombo()
{
	if (goToNextCombo)
	{
		goToNextCombo = false;

		switch (attackCount)
		{
		case 1:
			attackCount = 2;
			player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("Attack1")));
			TargetCheck(commonRange);
			break;
		case 2:
			attackCount = 0;
			player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("Attack2")));
			TargetCheck(commonRange);
			break;
		}
	}
}

void UAttackComponent::CommonAttack()
{
	if (!isAttacking && !player->playerAnim->bAir)
	{
		player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("Attack0")));
		attackCount = 1;
		player->GetCharacterMovement()->DisableMovement();
		isAttacking = true;
		TargetCheck(commonRange);

	}
	else
	{
		goToNextCombo = true;
	}
}

void UAttackComponent::intensiveAttack()
{
	if (!isAttacking && !player->playerAnim->bAir)
	{
		if (!coolTimeRunning)
		{
			player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("IntensiveAttack")));
			player->GetCharacterMovement()->DisableMovement();
			isAttacking = true;

			TargetCheck(IntensiveRange);
			coolTimeRunning = true;
			intensiveDelay = 5;

		}
	}
}

void UAttackComponent::SpecialAttack()
{
	if (!isAttacking && !player->playerAnim->bAir)
	{
		if (specialCount == 100)
		{
			player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("SpecialAttack")));
			player->GetCharacterMovement()->DisableMovement();
			isAttacking = true;
			TargetCheck(SpecialRange);
		}
	}
}

// SweepTrace�� ���� ������ �� �ִ��� �������� Ȯ���ϴ� �Լ�
// attackRadius�� ��ü �ݸ����� ������, attackLength�� ��ü �ݸ����� ����
bool UAttackComponent::CanAttack(float attackRadius, float attackLength)
{
	FCollisionShape attackCollision = FCollisionShape::MakeSphere(attackRadius);
	FCollisionQueryParams param;
	param.AddIgnoredActor(player);

	// �������� �� ī�޶� �ٶ󺸴� �������� ĳ���� ������
	FVector directionVector = player->GetActorLocation() - player->CamComp->GetComponentLocation();
	directionVector.Z = 0.0f;
	player->SetActorRotation(UKismetMathLibrary::MakeRotFromX(directionVector));

	// ����� ĸ�� �׸���
	FVector centerLoc = player->GetActorLocation() + player->GetActorForwardVector() * attackLength * 0.5f;
	float halfHeight = attackLength * 0.5f + attackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(player->GetActorForwardVector() * attackLength).ToQuat();
	DrawDebugCapsule(GetWorld(), centerLoc, halfHeight, attackCollision.GetSphereRadius(), capsuleRot, FColor::Cyan, false, 1.0f, 2.0f);

	bool bHits = GetWorld()->SweepMultiByChannel(hitinfos, player->GetActorLocation(), player->GetActorLocation() + player->GetActorForwardVector() * attackLength, FQuat::Identity, ECC_Visibility, attackCollision, param);

	if (bHits)
	{
		return true;
	}
	else return false;
}

// CanAttack �Լ��� True�� ��, �ε��� ���͵��� Ȯ���ϰ� �ش� ���͵��� �Լ��� ȣ���ϴ� �Լ�.
// attackRadius�� CanAttack �Լ����� ����� ��ü �ݸ����� ������, attackLength�� CanAttack �Լ����� ����� ��ü �ݸ����� ����, damage�� ReceiveDamage �Լ����� ����� �������� ��

void UAttackComponent::TargetCheck(FDamageRange damageRange)
{
	if (CanAttack(damageRange.attackRadius, damageRange.attackLength))
	{
		for (int32 i = 0; i < hitinfos.Num(); i++)
		{
			if (hitinfos[i].GetActor()->GetName().Contains(TEXT("Enemy")))
			{
				Target = Cast<AEnemyBase>(hitinfos[i].GetActor());
				if (Target != nullptr)
				{
					Target->FSM->ReceiveDamage(damageRange.damage);

					FVector force = -1.0f * Target->GetActorForwardVector() * damageRange.pushForce;
					Target->GetMesh()->AddForce(force, NAME_None, true);

					switch (damageRange.damageType)
					{
					case EDamageType::Common:
						intensiveDelay -= 1.0f;
						break;
					case  EDamageType::Intensive:
						specialCount += addPercent;
						specialCount = FMath::Clamp(specialCount, 0, 100);
						player->MainHUD->UpdateQPercent(specialCount);
						break;
					case EDamageType::Special:
						specialCount = 0;
						player->MainHUD->UpdateQPercent(specialCount);
						break;
					}
				}
				else if (hitinfos[i].GetActor()->GetName().Contains(TEXT("Hit")))
				{
					HitObject = Cast<AHitObjectBase>(hitinfos[i].GetActor());
					if (HitObject != nullptr)
					{
						HitObject->DropItem();
					}
				}
			}
		}
	}
}

void UAttackComponent::PushEnemy(float pushForce)
{
// 	FVector direction = Target->GetActorLocation() - player->GetActorLocation();
// 	FVector force = direction * pushForce;
// 
// 	Target->LaunchCharacter(force, true, true);
}
