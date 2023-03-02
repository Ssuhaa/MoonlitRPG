// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include "InventoryComponent.h"


// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT("ItemMesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));

}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::GetItem()
{
	if(Player != nullptr)
	{
		Player->InvenComp->CheckSameItem(ItemInfomation);
	}
}

