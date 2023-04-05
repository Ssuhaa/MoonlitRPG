// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include <Kismet/KismetMathLibrary.h>
#include "IH_InteractionUI.h"
#include "PlayerMainWG.h"
#include <UMG/Public/Components/VerticalBox.h>
#include <UMG/Public/Components/TextBlock.h>
#include "MainDialogueUI.h"
#include <UMG/Public/Components/Image.h>
#include <Camera/CameraComponent.h>


// Sets default values
ANPCBase::ANPCBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	SetRootComponent(capsuleComp);
	capsuleComp->SetCollisionObjectType(ECC_GameTraceChannel3);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Compoennt"));
	Mesh->SetupAttachment(RootComponent);

	NPCCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Interaction Camera"));
	NPCCamComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UIH_InteractionUI>tempinteractUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Interaction.WG_Interaction_C'"));
	if (tempinteractUI.Succeeded())
	{
		interactUIFactory = tempinteractUI.Class;
	}
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));

	interactionUI = CreateWidget<UIH_InteractionUI>(GetWorld(), interactUIFactory);
	interactionUI->txt_Interaction->SetText(FText::FromString(NPCName));
	interactionUI->img_Interact->SetBrushFromTexture(interactionImg);
}

// Called every frame
void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector direction = player->GetActorLocation() - GetActorLocation();
	float distance = FVector::Distance(GetActorLocation(), player->GetActorLocation());
	float dot = FVector::DotProduct(GetActorForwardVector(), direction.GetSafeNormal());
	float degree = UKismetMathLibrary::DegAcos(dot);

	if (!bTalking)
	{
		if (degree < 180 && distance <= 150)
		{
			if (!player->MainHUD->InteractionBox->GetAllChildren().Contains(interactionUI))
			{
				player->MainHUD->InteractionBox->AddChildToVerticalBox(interactionUI);
			}
		}
		else
		{
			player->MainHUD->InteractionBox->RemoveChild(interactionUI);
		}
	}
}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCBase::InteractNPC()
{
	if(interactionUI != nullptr)
	{
		interactionUI->RemoveFromParent();
	}

	bTalking = true;
	
	SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(player->GetActorLocation()-GetActorLocation(), FVector::UpVector));

	player->dialogueUI->npc = this;
	player->dialogueUI->ReadCSVFile(MakeCSVPath());
	player->dialogueUI->AddToViewport();
}

FString ANPCBase::MakeCSVPath()
{
	return FPaths::ProjectDir() + FString::Printf(TEXT("Dialogue/%s%d.csv"), *NPCName, DialogueNum); //CSV 파일명 Npc이름 + 번호

}
