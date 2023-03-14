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
#include "IH_DamageActor.h"
#include <UMG/Public/Components/WidgetComponent.h>

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Mouse_LeftClick.Mouse_LeftClick'"));
	if (tempAction.Succeeded())
	{
		inputarray.Add(tempAction.Object);  //0번 마우스 클릭
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction1(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_E.Key_E'"));
	if (tempAction1.Succeeded())
	{
		inputarray.Add(tempAction1.Object);  //1번 E키
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction2(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_Q.Key_Q'"));
	if (tempAction2.Succeeded())
	{
		inputarray.Add(tempAction2.Object);  //2번 Q키
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempPlayerMont(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Animations/Montage/AMT_Player.AMT_Player'"));
	if (tempPlayerMont.Succeeded())
	{
		playerMontage = tempPlayerMont.Object;
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
		EnhancedInputComponent->BindAction(inputarray[0], ETriggerEvent::Triggered, this, &UAttackComponent::CommonAttack); // 클릭
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
			{
				attackCount = 0;
				if (!bEquipGS)
				{
					PlayAttackMontage(playerMontage, "Attack1");
					TargetCheck(CommonRange);
				}
				else
				{
					PlayAttackMontage(playerMontage, "GS_Attack1");
				}
				break;
			}
		}
	}
}

void UAttackComponent::ResetAttack()
{
	isAttacking = false;
	goToNextCombo = false;
	isSpecialAttacking = false;
	attackCount = 0;
	player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void UAttackComponent::CommonAttack()
{
	if (player->bInventoryOpen == true) return;
	if (!isAttacking && !player->playerAnim->bAir)
	{
		if (!bEquipGS)
		{
			PlayAttackMontage(playerMontage, "Attack0");
			TargetCheck(CommonRange);
		}
		else
		{
			PlayAttackMontage(playerMontage, "GS_Attack0");
		}
		attackCount = 1;
	}
	else
	{
		goToNextCombo = true;
	}
}

void UAttackComponent::intensiveAttack()
{
	if (player->bInventoryOpen == true) return;
	if (!isAttacking && !player->playerAnim->bAir)
	{
		if (!coolTimeRunning)
		{
			if (!bEquipGS)
			{
				PlayAttackMontage(playerMontage, "IntensiveAttack");
			}
			else
			{
				PlayAttackMontage(playerMontage, "GS_IntensiveAttack");
			}
			coolTimeRunning = true;
			intensiveDelay = 5;
		}
	}
}

void UAttackComponent::SpecialAttack()
{
	if (player->bInventoryOpen == true) return;
	if (!isAttacking && !player->playerAnim->bAir && !isSpecialAttacking)
	{
		if (specialCount == 100)
		{	
			if (!bEquipGS)
			{
				PlayAttackMontage(playerMontage, "SpecialAttack");
			}
			else
			{
				PlayAttackMontage(playerMontage, "GS_SpecialAttack");
			}
			isSpecialAttacking = true;		// Q 스킬 쓸 때 피격당하지 않게 하기 위해서
		}
	}
}

void UAttackComponent::PlayAttackMontage(class UAnimMontage* montage, FString montName)		// 공격 몽타주를 재생하는 함수
{
	player->PlayAnimMontage(montage, 1.0f, FName(*montName));
	player->GetCharacterMovement()->DisableMovement();
	isAttacking = true;
}

void UAttackComponent::EnemyAttack(FDamageRange damageRange)	// Damage를 랜덤으로 뽑고 Enemy를 공격하는 함수
{
	DamageChange(damageRange);
	Target->FSM->ReceiveDamage(currDamage);

	direction = Target->GetActorLocation() - player->GetActorLocation();
	force = direction * damageRange.pushForce;
	force.Z = 0;
	Target->LaunchCharacter(force, true, true);
}

// attackRadius는 CanAttack 함수에서 사용할 구체 콜리전의 반지름, attackLength는 CanAttack 함수에서 사용할 구체 콜리전의 길이, damage는 ReceiveDamage 함수에서 사용할 데미지의 양
void UAttackComponent::TargetCheck(FDamageRange damageRange)
{
	FCollisionShape attackCollision = FCollisionShape::MakeSphere(damageRange.attackRadius);
	FCollisionQueryParams param;
	param.AddIgnoredActor(player);

	// 공격했을 때 카메라가 바라보는 방향으로 캐릭터 돌리기
// 	FVector directionVector = player->GetActorLocation() - player->CamComp->GetComponentLocation();
// 	directionVector.Z = 0.0f;
// 	player->SetActorRotation(UKismetMathLibrary::MakeRotFromX(directionVector));

	// 디버그 캡슐 그리기
	FVector centerLoc = player->GetActorLocation() + player->GetActorForwardVector() * damageRange.attackLength * 0.5f;
	float halfHeight = damageRange.attackLength * 0.5f + damageRange.attackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(player->GetActorForwardVector() * damageRange.attackLength).ToQuat();
	DrawDebugCapsule(GetWorld(), centerLoc, halfHeight, attackCollision.GetSphereRadius(), capsuleRot, FColor::Cyan, false, 1.0f, 2.0f);

	bool bHits = GetWorld()->SweepMultiByObjectType(hitinfos, player->GetActorLocation(), player->GetActorLocation() + player->GetActorForwardVector() * damageRange.attackLength, FQuat::Identity, ECC_GameTraceChannel1, attackCollision, param);

	if (bHits)
	{
		for (int32 i = 0; i < hitinfos.Num(); i++)
		{
			if (hitinfos[i].GetActor()->GetName().Contains(TEXT("Enemy")))
			{
				Target = Cast<AEnemyBase>(hitinfos[i].GetActor());

				if (Target != nullptr)
				{
					switch (damageRange.damageType)
					{
						case EDamageType::Common:
						{
 							intensiveDelay -= 1.0f;
							EnemyAttack(CommonRange);

							break;
						}
						case EDamageType::Intensive1:
						{
 							specialCount += addPercent;
							specialCount = FMath::Clamp(specialCount, 0, 100);
							player->MainHUD->UpdateQPercent(specialCount);

							EnemyAttack(IntensiveRange1);
							break;
						}
						case EDamageType::Intensive2:
						{
							EnemyAttack(IntensiveRange2);
							break;
						}
						case EDamageType::Special1:
						{
							specialCount = 0;
							player->MainHUD->UpdateQPercent(specialCount);
							EnemyAttack(SpecialRange1);
							break;
						}
						case EDamageType::Special2:
						{
							EnemyAttack(SpecialRange2);
							break;
						}
						case EDamageType::GS_Common:
						{
							intensiveDelay -= 1.0f;
							EnemyAttack(GS_CommonRange);
							break;
						}
						case EDamageType::GS_Intensive1:
						{
							specialCount += addPercent;
							specialCount = FMath::Clamp(specialCount, 0, 100);
							player->MainHUD->UpdateQPercent(specialCount);

							EnemyAttack(GS_IntensiveRange1);
							break;
						}
						case EDamageType::GS_Intensive2:
						{
							EnemyAttack(GS_IntensiveRange2);
							break;
						}
						case EDamageType::GS_Special1:
						{
							specialCount = 0;
							player->MainHUD->UpdateQPercent(specialCount);
							EnemyAttack(GS_SpecialRange1);
							break;
						}
						case EDamageType::GS_Special2:
						{
							EnemyAttack(GS_SpecialRange2);
							break;
						}
					}
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

void UAttackComponent::DamageChange(FDamageRange damageRangeType)
{
	iscriticAttack = FMath::RandRange(0.0f, 1.0f) < 0.2f;
	currDamage = FMath::RandRange(damageRangeType.minDamage, damageRangeType.maxDamage);

	if (iscriticAttack)
	{
		currDamage *= 3;
	}
}