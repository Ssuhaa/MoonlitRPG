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
#include "WeaponItemBase.h"
#include "InventoryComponent.h"
#include "IH_Puzzle.h"
#include <Particles/ParticleSystemComponent.h>

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
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempDamagedMont(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Animations/Montage/AMT_Player.AMT_Player'"));
	if (tempDamagedMont.Succeeded())
	{
		damagedMontage = tempDamagedMont.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempHandMont(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Animations/Montage/AMT_Hand.AMT_Hand'"));
	if (tempHandMont.Succeeded())
	{
		handMontage = tempHandMont.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempDaggerMont(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Animations/Montage/AMT_Player_Dagger.AMT_Player_Dagger'"));
	if (tempDaggerMont.Succeeded())
	{
		daggerMontage = tempDaggerMont.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempSwordMont(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Animations/Montage/AMT_Player_Sword.AMT_Player_Sword'"));
	if (tempDaggerMont.Succeeded())
	{
		swordMontage = tempSwordMont.Object;
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
			{
				PlayAttackMontage("Attack1");
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
	if (player->bUIOpen == true) return;
	if (!isAttacking && !player->playerAnim->bAir)
	{
		PlayAttackMontage("Attack0");
		attackCount = 1;
	}
	else
	{
		goToNextCombo = true;
	}
}

void UAttackComponent::intensiveAttack()
{
	if (player->bUIOpen == true) return;
	if (currWeapon == EWeaponType::None) return;
	if (!isAttacking && !player->playerAnim->bAir)
	{
		if (!coolTimeRunning)
		{
			PlayAttackMontage("IntensiveAttack");
			coolTimeRunning = true;
			intensiveDelay = 5;
		}
	}
}

void UAttackComponent::SpecialAttack()
{
	if (player->bUIOpen == true) return;
	if (currWeapon == EWeaponType::None) return;
	if (!isAttacking && !player->playerAnim->bAir && !isSpecialAttacking)
	{
		if (specialCount == 100)
		{	
			PlayAttackMontage("SpecialAttack");
			isSpecialAttacking = true;		// Q ��ų �� �� �ǰݴ����� �ʰ� �ϱ� ���ؼ�
		}
	}
}

void UAttackComponent::WeaponChange(EWeaponType weaponType)
{
	currWeapon = weaponType;
}

void UAttackComponent::ImpactEffect(FVector impactLoc)
{
	UParticleSystemComponent* compHitEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), player->hitImpact, impactLoc);
	compHitEffect->SetRelativeScale3D(FVector(0.5));
}

void UAttackComponent::PlayAttackMontage(FString montName)		// ���� ��Ÿ�ָ� ����ϴ� �Լ�
{
	player->GetCharacterMovement()->DisableMovement();
	isAttacking = true;

	player->playerAnim->currWeapon = currWeapon;
	player->playerAnim->bChangePose = true;

	switch (currWeapon)
	{
		case EWeaponType::None:
		{	
			player->PlayAnimMontage(handMontage, 1.0f, FName(*montName));
			break;
		}
		case EWeaponType::Dagger:
		{
			player->PlayAnimMontage(daggerMontage, 1.0f, FName(*montName));
			break;
		}
		case EWeaponType::Sword:
		{
			player->PlayAnimMontage(swordMontage, 1.0f, FName(*montName));
			break;
		}
	}
}

void UAttackComponent::EnemyAttack(FDamageRange damageRange)	// Damage�� �������� �̰� Enemy�� �����ϴ� �Լ�
{
	DamageChange(damageRange);

	switch (damageRange.damageType)
	{
		case EDamageType::DG_Intensive1:
		case EDamageType::DG_Special1:
		skillPercent = FMath::RandRange(1.3f, 1.4f);
		break;
		case EDamageType::DG_Intensive2:
		case EDamageType::DG_Special2:
		skillPercent = FMath::RandRange(1.5f, 1.6f);
		break;
		case EDamageType::GS_Intensive2:
		skillPercent = FMath::RandRange(2.0f, 2.2f);
		break;
		case EDamageType::GS_Special1:
		skillPercent = FMath::RandRange(2.5f, 2.6f);
		break;
	}

	currDamage *= skillPercent;
	skillPercent = 1.0f;	// �⺻ ������ ���� �ٽ� �ʱ�ȭ

	Target->FSM->ReceiveDamage(currDamage);                                                                                                                                                           

	if (Target->FSM->currHP > 0)
	{
		direction = Target->GetActorLocation() - player->GetActorLocation();
		force = direction * damageRange.pushForce;
		force.Z = 0;
		Target->LaunchCharacter(force, true, true);
	}
}

// attackRadius�� CanAttack �Լ����� ����� ��ü �ݸ����� ������, attackLength�� CanAttack �Լ����� ����� ��ü �ݸ����� ����, damage�� ReceiveDamage �Լ����� ����� �������� ��
void UAttackComponent::TargetCheck(FDamageRange damageRange)
{
	FCollisionShape attackCollision = FCollisionShape::MakeSphere(damageRange.attackRadius);
	FCollisionQueryParams param;
	param.AddIgnoredActor(player);

	// �������� �� ī�޶� �ٶ󺸴� �������� ĳ���� ������
// 	FVector directionVector = player->GetActorLocation() - player->CamComp->GetComponentLocation();
// 	directionVector.Z = 0.0f;
// 	player->SetActorRotation(UKismetMathLibrary::MakeRotFromX(directionVector));

	// ����� ĸ�� �׸���
	FVector centerLoc = player->GetActorLocation() + player->GetActorForwardVector() * damageRange.attackLength * 0.5f;
	float halfHeight = damageRange.attackLength * 0.5f + damageRange.attackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(player->GetActorForwardVector() * damageRange.attackLength).ToQuat();
	// DrawDebugCapsule(GetWorld(), centerLoc, halfHeight, attackCollision.GetSphereRadius(), capsuleRot, FColor::Cyan, false, 1.0f, 2.0f);

	bool bHits = GetWorld()->SweepMultiByObjectType(hitinfos, player->GetActorLocation(),
	player->GetActorLocation() + player->GetActorForwardVector() * damageRange.attackLength, FQuat::Identity, ECC_GameTraceChannel1, attackCollision, param);

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
						case EDamageType::Hand_Common:
						{
							EnemyAttack(CommonRange);
							break;
						}
						// ************************************* �ܰ� ����
						case EDamageType::DG_Common:
						{
 							intensiveDelay -= 1.0f;
							EnemyAttack(DG_CommonRange);

							break;
						}
						case EDamageType::DG_Intensive1:
						{
 							specialCount += addPercent;
							specialCount = FMath::Clamp(specialCount, 0, 100);
							player->MainHUD->UpdateQPercent(specialCount);

							EnemyAttack(DG_IntensiveRange1);
							break;
						}
						case EDamageType::DG_Intensive2:
						{
							EnemyAttack(DG_IntensiveRange2);
							break;
						}
						case EDamageType::DG_Special1:
						{
							specialCount = 0;
							player->MainHUD->UpdateQPercent(specialCount);
							EnemyAttack(DG_SpecialRange1);
							break;
						}
						case EDamageType::DG_Special2:
						{
							EnemyAttack(DG_SpecialRange2);
							break;
						}
						// ************************************* �μհ� ����
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

				if (currWeapon != EWeaponType::None &&  HitObject != nullptr)
				{
					ImpactEffect(HitObject->GetActorLocation());
					HitObject->DropItem();
				}
			}
			else if (hitinfos[i].GetActor()->GetName().Contains(TEXT("Puzzle")))
			{
				HitPuzzle = Cast<AIH_Puzzle>(hitinfos[i].GetActor());

				if (HitPuzzle != nullptr)
				{
					UStaticMeshComponent* HitMesh = Cast<UStaticMeshComponent>(hitinfos[i].GetComponent());
					if (HitMesh != nullptr)
					{
						ImpactEffect(HitMesh->GetComponentLocation());
						if (HitPuzzle->hitMeshArr.Num() < HitPuzzle->meshArr.Num())
						{
							HitPuzzle->ReceiveMeshArr(HitMesh);
//							UE_LOG(LogTemp, Warning, TEXT("Hit Mesh : %s"), *HitMesh->GetName());
						}
					}
				}
			}
		}
	}
}

void UAttackComponent::DamageChange(FDamageRange damageRangeType)
{
	int32 index = player->InvenComp->CheckWeaponisEquip();
	float currPower = 0;

	if (index > -1)	// ���⸦ �����ϰ� ������
	{
		currPower = player->InvenComp->invenItemArr[index].weaponinfomaiton.Power;	// �ش� ������ ���ݷ��� �����Ѵ�.
	}

	// ������ ���ݷ¸�ŭ ���� ���� �������� �̴´�.
	currDamage = FMath::RandRange(damageRangeType.minDamage+int32(currPower), damageRangeType.maxDamage+int32(currPower));
	float randPercent = FMath::RandRange(0.7f, 1.0f);
	currDamage*=randPercent;	// 0.7~1.0�谡 ���� ���ݷ��� �ȴ�.

	iscriticAttack = FMath::RandRange(0.0f, 1.0f) < 0.2f;
	if (iscriticAttack)		// 20% ������ �� ũ��Ƽ�� �������� ������.
	{
		currDamage *= 3;
	}
}

void UAttackComponent::SkillDamageChange(FDamageRange damageRangeType)
{
	int32 index = player->InvenComp->CheckWeaponisEquip();
	float currPower = 0;

	if (index > -1)	// ���⸦ �����ϰ� ������
	{
		currPower = player->InvenComp->invenItemArr[index].weaponinfomaiton.Power;	// �ش� ������ ���ݷ��� �����Ѵ�.
	}

	// ������ ���ݷ¸�ŭ ���� ���� �������� �̴´�.
	currDamage = FMath::RandRange(damageRangeType.minDamage + int32(currPower), damageRangeType.maxDamage + int32(currPower));
	float randPercent = FMath::RandRange(1.2f, 1.3f);
	currDamage *= randPercent;	// 0.7~1.0�谡 ���� ���ݷ��� �ȴ�.

	iscriticAttack = FMath::RandRange(0.0f, 1.0f) < 0.2f;
	if (iscriticAttack)		// 20% ������ �� ũ��Ƽ�� �������� ������.
	{
		currDamage *= 3;
	}
}