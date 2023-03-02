// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObjectBase.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include "ItemBase.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include "SH_Player.h"
#include "IH_InteractionUI.h"

// Sets default values
AInteractiveObjectBase::AInteractiveObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(compMesh);

	compSpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Position Component"));
	compSpawnPos->SetupAttachment(RootComponent);
	compSpawnPos->SetRelativeLocation(FVector(0, 0, 30));

	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	compSphere->SetupAttachment(RootComponent);
	compSphere->SetSphereRadius(300.0);

	compInteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interact Widget Component"));
	compInteractWidget->SetupAttachment(RootComponent);
	compInteractWidget->SetRelativeLocation(FVector(0, 0, 50));
	compInteractWidget->SetVisibility(false);

	ConstructorHelpers::FClassFinder<UIH_InteractionUI>tempinteractionUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Interaction.WG_Interaction_C'"));
	if (tempinteractionUI.Succeeded())
	{
		compInteractWidget->SetWidgetClass(tempinteractionUI.Class);
		compInteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	}
}

// Called when the game starts or when spawned
void AInteractiveObjectBase::BeginPlay()
{
	Super::BeginPlay();

	compSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveObjectBase::FloatInteract);
	compSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractiveObjectBase::RemoveInteract);
}

// Called every frame
void AInteractiveObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveObjectBase::DropItem()
{
	float randZ = FMath::RandRange(0, 360);
	compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));

	float randDist = FMath::RandRange(60, 100);
	FVector randForward = compSpawnPos->GetForwardVector()*randDist;
	compSpawnPos->SetRelativeLocation(compSpawnPos->GetComponentLocation()+randForward);

	if (spawnItems.IsValidIndex(0))
	{
		for (int32 i = 1; i <= spawnItems.Num(); i++)
		{
			int32 randNum = FMath::RandRange(0, spawnItems.Num()-1);
			GetWorld()->SpawnActor<AItemBase>(spawnItems[randNum], compSpawnPos->GetComponentLocation(), compSpawnPos->GetComponentRotation());
		}
	}
}

void AInteractiveObjectBase::FloatInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		player = Cast<ASH_Player>(OtherActor);
		if (OtherActor == player)
		{
			compInteractWidget->SetVisibility(true);
		}
	}
}

void AInteractiveObjectBase::RemoveInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		player = Cast<ASH_Player>(OtherActor);
		if (OtherActor == player)
		{
			compInteractWidget->SetVisibility(false);
		}
	}
}