// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObjectBase.h"
#include <Components/StaticMeshComponent.h>
#include "ItemBase.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include "SH_Player.h"
#include "IH_InteractionUI.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/VerticalBox.h>
#include "PlayerMainWG.h"

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

	compInteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interact Widget Component"));
	compInteractWidget->SetupAttachment(RootComponent);
	compInteractWidget->SetRelativeLocation(FVector(0, 0, 50));
	compInteractWidget->SetVisibility(false);

	ConstructorHelpers::FClassFinder<UIH_InteractionUI>tempinteractionUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Interaction.WG_Interaction_C'"));
	if (tempinteractionUI.Succeeded())
	{
		interactUIFactory = tempinteractionUI.Class;
		compInteractWidget->SetWidgetClass(tempinteractionUI.Class);
		compInteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	}
}

// Called when the game starts or when spawned
void AInteractiveObjectBase::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(),ASH_Player::StaticClass()));
	interactionUI = CreateWidget<UIH_InteractionUI>(GetWorld(), interactUIFactory);
//	UIH_InteractionUI* interactionUI = Cast<UIH_InteractionUI>(compInteractWidget->GetUserWidgetObject());
	interactionUI->txt_Interaction->SetText(InteractName);
}

// Called every frame
void AInteractiveObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector targetVector = player->GetActorLocation() - GetActorLocation();
	float dot = FVector::DotProduct(GetActorForwardVector(), targetVector.GetSafeNormal());
	float degree = UKismetMathLibrary::DegAcos(dot);
	float distance = FVector::Distance(player->GetActorLocation(), GetActorLocation());
	
		if (degree < 180 && distance < 300)
		{
			player->MainHUD->InteractionBox->AddChildToVerticalBox(interactionUI);
//			compInteractWidget->SetVisibility(true);
		}
		else
		{
			player->MainHUD->InteractionBox->RemoveChild(interactionUI);
//			compInteractWidget->SetVisibility(false);
		}
}

void AInteractiveObjectBase::Interaction()
{
	float randZ = FMath::RandRange(0, 360);
	compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));

// 	float randDist = FMath::RandRange(10, 30);
// 	FVector randForward = compSpawnPos->GetForwardVector()*randDist;
// 	compSpawnPos->SetRelativeLocation(compSpawnPos->GetComponentLocation()+randForward);

	if (spawnItems.IsValidIndex(0))
	{
		int32 randAmount = FMath::RandRange(3, 5);
		for (int32 i = 1; i <= randAmount; i++)
		{
			int32 randNum = FMath::RandRange(0, spawnItems.Num()-1);
			GetWorld()->SpawnActor<AItemBase>(spawnItems[randNum], compSpawnPos->GetComponentLocation(), compSpawnPos->GetComponentRotation());
		}
	}

	if (interactionUI != nullptr)
	{
		interactionUI->RemoveFromParent();
	}

	Destroy();
}