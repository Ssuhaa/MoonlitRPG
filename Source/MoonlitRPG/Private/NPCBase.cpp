// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ANPCBase::ANPCBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	SetRootComponent(capsuleComp);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Compoennt"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
}

// Called every frame
void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector forward = GetActorForwardVector();
	FVector direction = player->GetActorLocation() - GetActorLocation();
	float distance = direction.Length();
	float dot = FVector::DotProduct(forward, direction.GetSafeNormal());
	float degree = UKismetMathLibrary::DegAcos(dot);

	if (distance <= 500 && degree < 180)
	{

	}
}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

