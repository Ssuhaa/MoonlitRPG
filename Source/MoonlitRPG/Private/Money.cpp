// Fill out your copyright notice in the Description page of Project Settings.


#include "Money.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "InventoryComponent.h"

// Sets default values
AMoney::AMoney()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(compMesh);
}

// Called when the game starts or when spawned
void AMoney::BeginPlay()
{
	Super::BeginPlay();
	
	compMesh->SetSimulatePhysics(true);
}

// Called every frame
void AMoney::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currTime += DeltaTime;

	if (currTime > 1)
	{
		if (compMesh->IsSimulatingPhysics())
		{
			compMesh->SetSimulatePhysics(false);
		}

		FVector P0 = GetActorLocation();
		FVector V = player->GetActorLocation() - GetActorLocation();
		FVector Vt = V * DeltaTime * power;
		FVector P = P0 + Vt;
	
		SetActorLocation(P);
	}
}

void AMoney::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor != this)
	{
		player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));

		if (OtherActor == player)
		{
			int32 randMoney = FMath::RandRange(minMoney, maxMoney);
			player->InvenComp->Money += randMoney;
			Destroy();
		}
	}
}