// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Player.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "MoveComponent.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

ASH_Player::ASH_Player()
{
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	CamComp->SetupAttachment(SpringArmComp);

	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = true;
	
}

void ASH_Player::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	playerCon->PlayerCameraManager->ViewPitchMin = -10.0f;
	playerCon->PlayerCameraManager->ViewPitchMax = 50.0f;
}

void ASH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASH_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent != nullptr)
	{
		MoveComp->SetupPlayerInputComponent(EnhancedInputComponent);
	}
}
