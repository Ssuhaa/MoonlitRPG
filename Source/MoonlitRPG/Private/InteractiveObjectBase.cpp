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
#include <UMG/Public/Components/Image.h>

// Sets default values
AInteractiveObjectBase::AInteractiveObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(compRoot);

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	compMesh->SetupAttachment(RootComponent);
	compMesh->SetCollisionObjectType(ECC_GameTraceChannel3);

	compSpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Position Component"));
	compSpawnPos->SetupAttachment(RootComponent);
	compSpawnPos->SetRelativeLocation(FVector(0, 0, 50));

	ConstructorHelpers::FClassFinder<UIH_InteractionUI>tempinteractionUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Interaction.WG_Interaction_C'"));
	if (tempinteractionUI.Succeeded())
	{
		interactUIFactory = tempinteractionUI.Class;
	}
}

// Called when the game starts or when spawned
void AInteractiveObjectBase::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(),ASH_Player::StaticClass()));

	interactionUI = CreateWidget<UIH_InteractionUI>(GetWorld(), interactUIFactory);
	interactionUI->txt_Interaction->SetText(InteractName);
	interactionUI->img_Interact->SetBrushFromTexture(interactionImg);
	originPos = compSpawnPos->GetComponentLocation();
}

// Called every frame
void AInteractiveObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector targetVector = player->GetActorLocation() - GetActorLocation();
	float dot = FVector::DotProduct(GetActorForwardVector(), targetVector.GetSafeNormal());
	float degree = UKismetMathLibrary::DegAcos(dot);
	float distance = FVector::Distance(player->GetActorLocation(), GetActorLocation());

	if (degree < 180 && distance <= 150)
	{
		if (!player->MainHUD->InteractionBox->GetAllChildren().Contains(interactionUI))
		{
			player->MainHUD->InteractionBox->AddChildToVerticalBox(interactionUI);
		}
	}
	else
	{
		if (player->MainHUD->InteractionBox->GetAllChildren().Contains(interactionUI))
		{
			player->MainHUD->InteractionBox->RemoveChild(interactionUI);
		}
	}
}

void AInteractiveObjectBase::Interaction()
{
	if (spawnMoney.IsValidIndex(0))
	{
		int32 randCoin = FMath::RandRange(2, 3);

		for (int32 i = 1; i <= randCoin; i++)
		{
			float randZ = FMath::RandRange(0, 360);
			compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));

			GetWorld()->SpawnActor<AMoney>(spawnMoney[0], compSpawnPos->GetComponentLocation(), compSpawnPos->GetComponentRotation());
		}
	}

	if (spawnItems.IsValidIndex(0))
	{
		int32 randAmount = FMath::RandRange(3, 4);		// °³¼ö ·£´ý»Ì±â

		for (int32 i = 1; i <= randAmount; i++)
		{
			float randZ = FMath::RandRange(0, 360);
			compSpawnPos->SetRelativeRotation(FRotator(0, randZ, 0));

			int32 randIndex = FMath::RandRange(0, spawnItems.Num()-1);		// ¹è¿­ ¿ä¼Ò ·£´ý»Ì±â
			GetWorld()->SpawnActor<AItemBase>(spawnItems[randIndex], compSpawnPos->GetComponentLocation(), compSpawnPos->GetComponentRotation());
		}
	}

	if (interactionUI != nullptr)
	{
		interactionUI->RemoveFromParent();
	}
}