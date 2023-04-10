// Fill out your copyright notice in the Description page of Project Settings.


#include "MagoPiece.h"
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>

AMagoPiece::AMagoPiece()
{
	PrimaryActorTick.bCanEverTick = true;

	compMago = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mago Parts Component"));
	compMago->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempEffect1(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/Stylized_Mobile_Effects/Particles/P_Loot_7.P_Loot_7'"));
	if (tempEffect1.Succeeded())
	{
		magoEffect.Add(tempEffect1.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem>tempEffect2(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/GoodFXLevelUp/FX/Particles/PS_GFXLU_Galaxy.PS_GFXLU_Galaxy'"));
	if (tempEffect2.Succeeded())
	{
		magoEffect.Add(tempEffect2.Object);
	}

	floatSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Floating Sound Component"));
	floatSoundComp->SetupAttachment(RootComponent);
}

void AMagoPiece::BeginPlay()
{
	Super::BeginPlay();
	
	itemEffect->SetRelativeScale3D(FVector(1.0));
	itemEffect->SetTemplate(magoEffect[0]);

	interactionUI=nullptr;

	originPos = GetActorLocation();
}

void AMagoPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	rotateZ += DeltaTime * 100;
	compMago->SetWorldRotation(FRotator(0, rotateZ, 0));

	float period = 3.0f;
	float amplitude = 10.0f;

	float DeltaMove = amplitude*FMath::Sin(2*PI*GetWorld()->GetTimeSeconds()/period);

	FVector NewLoc = originPos + FVector(0.0f, 0.0f, DeltaMove);

	SetActorLocation(NewLoc);
}

void AMagoPiece::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ASH_Player* player = Cast<ASH_Player>(OtherActor);
	
	if (player != nullptr)
	{
		GetItem();
	}
}

void AMagoPiece::GetItem()
{
	Super::GetItem();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), magoEffect[1], GetActorLocation(), GetActorRotation());
}
