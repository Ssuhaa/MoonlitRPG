// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include "SH_Player.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerMainWG.h"
#include <Animation/AnimMontage.h>

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder <UInputMappingContext> tempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/input/IMC_Player.IMC_Player'"));
	if(tempIMC.Succeeded())
	{
		IMC = tempIMC.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_AD.Key_AD'"));
	if (tempAction.Succeeded())
	{
		inputArray.Add(tempAction.Object); //0번 AD 
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction1(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_WS.Key_WS'"));
	if (tempAction1.Succeeded())
	{
		inputArray.Add(tempAction1.Object); //1번 WS
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction2(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Mouse_XY.Mouse_XY'"));
	if (tempAction2.Succeeded())
	{
		inputArray.Add(tempAction2.Object);//2번 마우스 XY
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction3(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_Space.Key_Space'"));
	if (tempAction3.Succeeded())
	{
		inputArray.Add(tempAction3.Object);//3번 Space
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction4(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_Shift.Key_Shift'"));
	if (tempAction4.Succeeded())
	{
		inputArray.Add(tempAction4.Object);//4번 Shift
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Animations/Montage/AMT_Move.AMT_Move'"));
	if (tempMontage.Succeeded())
	{
		MoveMontage = tempMontage.Object;
	}
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playercontroller = GetWorld()->GetFirstPlayerController();
	Player = Cast<ASH_Player>(GetOwner());
	Movement = Player->GetCharacterMovement();
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playercontroller->GetLocalPlayer());

	subSystem->AddMappingContext(IMC, 0);
	
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	dir = FTransform(Player->GetControlRotation()).TransformVector(dir);
	Player->AddMovementInput(dir.GetSafeNormal());
	dir = FVector::ZeroVector;
	isPlayerMove = PlayerMoveCheck();

	StaminaUpdate();
	DashToWalk();

}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent)
{
	if (inputArray.IsValidIndex(0))
	{
		EnhancedInputComponent->BindAction(inputArray[0],ETriggerEvent::Triggered, this, &UMoveComponent::Horizontal); // 0번 AD
		EnhancedInputComponent->BindAction(inputArray[1],ETriggerEvent::Triggered,  this, &UMoveComponent::Vertical); // 1번 WS
		EnhancedInputComponent->BindAction(inputArray[2],ETriggerEvent::Triggered,  this, &UMoveComponent::Look); // 2번 마우스XY
		EnhancedInputComponent->BindAction(inputArray[3],ETriggerEvent::Triggered, this, &UMoveComponent::Jump); // 3번 Space 
		EnhancedInputComponent->BindAction(inputArray[4],ETriggerEvent::Started, this, &UMoveComponent::Dash); //4번 Shift
	}

}


void UMoveComponent::Horizontal(const FInputActionValue& value)
{
	if(Player->bInventoryOpen == true) return;
	float Axis = value.Get<float>();
	dir.Y += Axis;
}

void UMoveComponent::Vertical(const FInputActionValue& value)
{
	if (Player->bInventoryOpen == true) return;
	float Axis = value.Get<float>();
	dir.X += Axis;
}

void UMoveComponent::Look(const FInputActionValue& value)
{
	if (Player->bInventoryOpen == true) return;
	FVector2D MouseAxis = value.Get<FVector2D>();

	Player->AddControllerYawInput(MouseAxis.X);
	Player->AddControllerPitchInput(-MouseAxis.Y);
}

void UMoveComponent::Jump()
{
	if (Player->bInventoryOpen == true) return;
	Player->Jump();
}


void UMoveComponent::Dash()
{
	if (Player->bInventoryOpen == true) return;
	if (Stamina > 30)
	{
		isStaminaUse = true;
		CurrSpeed = DashSpeed;
	}
}



void UMoveComponent::DashToWalk()
{
	if (isStaminaUse)
	{
		if (Stamina > 10)
		{
			SetWalkSpped(RunSpeed, DashSpeed, 10);
		}
		else
		{
			SetWalkSpped(WalkSpeed, DashSpeed, 10);
			if (Stamina < 1)
			{
				isStaminaUse = false;
			}
		}
	}
	else
	{
		SetWalkSpped(WalkSpeed, DashSpeed, 10);
		isStaminaUse = false;
	}
}

void UMoveComponent::SetWalkSpped(float MinSpeed, float MaxSpeed, float MinusSpeed)
{
	CurrSpeed -= MinusSpeed;
	CurrSpeed = FMath::Clamp(CurrSpeed, MinSpeed, MaxSpeed);
	Movement->MaxWalkSpeed = CurrSpeed;
}

void UMoveComponent::StaminaUpdate()
{
	if (isStaminaUse)
	{
		if (isPlayerMove)
		{
			Stamina -= decreaseStamina;
			Stamina = FMath::Clamp(Stamina, minStamina, maxStamina);
		}
		else 
		{
			Stamina += decreaseStamina;
			Stamina = FMath::Clamp(Stamina, minStamina, maxStamina);
			isStaminaUse = false;
		}
	}
	else
	{
		Stamina += decreaseStamina;
		Stamina = FMath::Clamp(Stamina, minStamina, maxStamina);
	}

	if (Player->MainHUD != nullptr)
	{
		Player->MainHUD->UpdateStamina(Stamina, maxStamina);
	}
}

bool UMoveComponent::PlayerMoveCheck()
{
	if (Player != nullptr)
	{
		FVector velocity = Player->GetVelocity();
		FVector forwardVector = Player->GetActorForwardVector();
		dirV = FVector::DotProduct(forwardVector, velocity);

		FVector rightVector = Player->GetActorRightVector();
		dirH = FVector::DotProduct(rightVector, velocity);

		isAir = Player->GetCharacterMovement()->IsFalling();
		if (dirH != 0 || dirV != 0)
		{
			return true;
		}
	}
	return false;
}
