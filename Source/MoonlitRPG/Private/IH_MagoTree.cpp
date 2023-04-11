// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_MagoTree.h"
#include "DangsanWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include "PlayerMainWG.h"
#include <UMG/Public/Components/VerticalBox.h>
#include "IH_InteractionUI.h"


AIH_MagoTree::AIH_MagoTree()
{
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	compBox->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UDangsanWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_DangSan.BP_DangSan_C'"));
	if (tempWG.Succeeded())
	{
		WGFactory = tempWG.Class;
	}
	WG_Dangsan = CreateWidget<UDangsanWG>(GetWorld(), WGFactory);
}

void AIH_MagoTree::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(),ASH_Player::StaticClass()));
	compBox->OnComponentBeginOverlap.AddDynamic(this, &AIH_MagoTree::CanOffering);
	compBox->OnComponentEndOverlap.AddDynamic(this, &AIH_MagoTree::CantOffering);
}

void AIH_MagoTree::Tick(float DeltaTime)
{

}

void AIH_MagoTree::Interaction()
{
	if (Player->bUIOpen == false && !WG_Dangsan->IsInViewport())
	{
		WG_Dangsan->AddToViewport();
	}
	else
	{
		WG_Dangsan->RemoveWG();
	}
}

void AIH_MagoTree::CanOffering(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherActor == Player)
		{
			if (!Player->MainHUD->InteractionBox->GetAllChildren().Contains(interactionUI))
			{
				player->magoTree = this;
				player->MainHUD->InteractionBox->AddChildToVerticalBox(interactionUI);
			}
		}
	}
}

void AIH_MagoTree::CantOffering(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		if (OtherActor == Player)
		{
			if (Player->MainHUD->InteractionBox->GetAllChildren().Contains(interactionUI))
			{
				player->magoTree = nullptr;
				player->MainHUD->InteractionBox->RemoveChild(interactionUI);
			}
		}
	}
}