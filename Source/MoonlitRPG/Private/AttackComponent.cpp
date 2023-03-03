// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy_FSM.h"
#include "IH_Enemy.h"
#include "SH_Player.h"
#include <Kismet/KismetMathLibrary.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
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
	//Target = Cast<AIH_Enemy>(UGameplayStatics::GetActorOfClass(GetWorld(), AIH_Enemy::StaticClass()));
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
		UE_LOG(LogTemp, Warning, TEXT("gotoNextCombo"));
		goToNextCombo = false;

		attackCount++;
		UE_LOG(LogTemp, Warning, TEXT("%d"), attackCount);

		switch (attackCount)
		{
		case 1:
			attackCount = 2;
			player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("Attack1")));
			break;
		case 2:
			attackCount = 0;
			player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("Attack2")));
			break;
		}
	}
}

void UAttackComponent::CommonAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("CommonAttack"));

	if (!isAttacking)
	{
		player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("Attack0")));
		player->GetCharacterMovement()->DisableMovement();
		isAttacking = true;

		if (CanAttack(50, 80))
		{
			for (int32 i = 0; i < hitinfos.Num(); i++)
			{
				if (hitinfos[i].GetActor()->GetName().Contains(TEXT("Enemy")))
				{
					Target = Cast<AIH_Enemy>(hitinfos[i].GetActor());
					if (Target != nullptr)
					{
						Target->FSM->ReceiveDamage(1);
					}
				}
			}
		}
	}
	else
	{
		goToNextCombo = true;
	}
}

void UAttackComponent::intensiveAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("intensiveAttack"));

	if (!isAttacking)
	{
		player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("IntensiveAttack")));
		player->GetCharacterMovement()->DisableMovement();
		isAttacking = true;

		if (CanAttack(80, 100))
		{
			for (int32 i = 0; i < hitinfos.Num(); i++)
			{
				if (hitinfos[i].GetActor()->GetName().Contains(TEXT("Enemy")))
				{
					Target = Cast<AIH_Enemy>(hitinfos[i].GetActor());
					if (Target != nullptr)
					{
						Target->FSM->ReceiveDamage(3);
					}
				}
			}
		}
	}
}

void UAttackComponent::SpecialAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("SpecialAttack"));

	if (!isAttacking)
	{
		player->PlayAnimMontage(attackMontage, 1.0f, FName(TEXT("SpecialAttack")));
		player->GetCharacterMovement()->DisableMovement();
		isAttacking = true;

		if (CanAttack(150, 100))
		{
			for (int32 i = 0; i < hitinfos.Num(); i++)
			{
				if (hitinfos[i].GetActor()->GetName().Contains(TEXT("Enemy")))
				{
					Target = Cast<AIH_Enemy>(hitinfos[i].GetActor());
					if (Target != nullptr)
					{
						Target->FSM->ReceiveDamage(5);
					}
				}
			}
		}
	}
}

bool UAttackComponent::CanAttack(float attackRadius, float attackLength)
{
	FCollisionShape attackCollision = FCollisionShape::MakeSphere(attackRadius);
	FCollisionQueryParams param;
	param.AddIgnoredActor(player);

	// 공격했을 때 카메라가 바라보는 방향으로 캐릭터 돌리기
	FVector directionVector = player->GetActorLocation() - player->CamComp->GetComponentLocation();
	directionVector.Z = 0.0f;
	player->SetActorRotation(UKismetMathLibrary::MakeRotFromX(directionVector));

	// 디버그 캡슐 그리기
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