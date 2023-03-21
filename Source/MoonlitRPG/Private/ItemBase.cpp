// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include "InventoryComponent.h"
#include "IH_InteractionUI.h"
#include <UMG/Public/Components/TextBlock.h>
#include <Kismet/KismetMathLibrary.h>
#include "PlayerMainWG.h"
#include <UMG/Public/Components/VerticalBox.h>
#include "IH_GetItemUI.h"
#include <UMG/Public/Components/Image.h>


// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT("ItemMesh"));
	SetRootComponent(Mesh);
	Mesh->SetRelativeLocation(FVector(0));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel3);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);

	ConstructorHelpers::FClassFinder<UIH_InteractionUI>tempinteractUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_Interaction.WG_Interaction_C'"));
	if (tempinteractUI.Succeeded())
	{
		interactUIFactory.Add(tempinteractUI.Class);
	}

	ConstructorHelpers::FClassFinder<UIH_GetItemUI>tempgetItemUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_GetItem.WG_GetItem_C'"));
	if (tempinteractUI.Succeeded())
	{
		interactUIFactory.Add(tempgetItemUI.Class);
	}
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));

	interactionUI = CreateWidget<UIH_InteractionUI>(GetWorld(), interactUIFactory[0]);
	interactionUI->txt_Interaction->SetText(FText::FromString(ItemInformation.ItemName));
	interactionUI->img_Interact->SetBrushFromTexture(ItemInformation.itemImage);

	getItemUI = CreateWidget< UIH_GetItemUI>(GetWorld(), interactUIFactory[1]);
	getItemUI->txt_ItemName->SetText(FText::FromString(FString::Printf(TEXT("%s x %d"), *ItemInformation.ItemName, 1)));
	getItemUI->img_Get->SetBrushFromTexture(ItemInformation.itemImage);
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector targetVector = Player->GetActorLocation() - GetActorLocation();
	float dot = FVector::DotProduct(GetActorForwardVector(), targetVector.GetSafeNormal());
	float degree = UKismetMathLibrary::DegAcos(dot);
	float distance = FVector::Distance(Player->GetActorLocation(), GetActorLocation());

	if (degree < 180 && distance <= 150)
	{
		if (!bAddWidget)
		{
			int32 widgetCount = Player->MainHUD->InteractionBox->GetChildrenCount();

			if (widgetCount <= 6)
			{
				Player->MainHUD->InteractionBox->AddChildToVerticalBox(interactionUI);
				bAddWidget = true;
			}
		}
	}
	else
	{
		if (bAddWidget)
		{
			Player->MainHUD->InteractionBox->RemoveChild(interactionUI);
			bAddWidget = false;
		}
	}
}

void AItemBase::GetItem()
{
	if(Player != nullptr)
	{
		Player->InvenComp->CheckSameItemAfterAdd(ItemInformation, 1);

		int32 widgetCount = Player->MainHUD->ItemGetBox->GetChildrenCount();

		if (widgetCount < 7)
		{
			Player->MainHUD->ItemGetBox->AddChild(getItemUI);
		}

		if (interactionUI != nullptr)
		{
			interactionUI->RemoveFromParent();
		}

		Destroy();
	}
}

