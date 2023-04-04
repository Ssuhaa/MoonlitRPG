// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_MagoTree.h"
#include "DangsanWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>

AIH_MagoTree::AIH_MagoTree()
{
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