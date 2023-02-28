// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include "SH_Player.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>

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

}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playercontroller = GetWorld()->GetFirstPlayerController();
	Player = Cast<ASH_Player>(GetOwner());
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

}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(inputArray[0],ETriggerEvent::Triggered, this, &UMoveComponent::Horizontal); // 0번 AD
	EnhancedInputComponent->BindAction(inputArray[1],ETriggerEvent::Triggered,  this, &UMoveComponent::Vertical); // 1번 WS
	EnhancedInputComponent->BindAction(inputArray[2],ETriggerEvent::Triggered,  this, &UMoveComponent::Look); // 2번 마우스XY
	EnhancedInputComponent->BindAction(inputArray[3],ETriggerEvent::Triggered, this, &UMoveComponent::Jump); // 3번 Space
}


void UMoveComponent::Horizontal(const FInputActionValue& value)
{
	float Axis = value.Get<float>();
	dir.Y += Axis;
}

void UMoveComponent::Vertical(const FInputActionValue& value)
{
	float Axis = value.Get<float>();
	dir.X += Axis;
}

void UMoveComponent::Look(const FInputActionValue& value)
{
	FVector2D MouseAxis = value.Get<FVector2D>();

	Player->AddControllerYawInput(MouseAxis.X);
	Player->AddControllerPitchInput(-MouseAxis.Y);
}

void UMoveComponent::Jump()
{
	Player->Jump();
}
