// Fill out your copyright notice in the Description page of Project Settings.


#include "Money.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "InventoryComponent.h"
#include "IH_GetItemUI.h"
#include "PlayerMainWG.h"
#include <UMG/Public/Components/VerticalBox.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/Image.h>

// Sets default values
AMoney::AMoney()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(compMesh);

	ConstructorHelpers::FClassFinder<UIH_GetItemUI>tempgetUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_GetItem.WG_GetItem_C'"));
	if (tempgetUI.Succeeded())
	{
		getitemUIFactory = tempgetUI.Class;
	}

	ConstructorHelpers::FObjectFinder<UTexture2D>temptexture(TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/UI_money.UI_money'"));
	if (temptexture.Succeeded())
	{
		coinImage = temptexture.Object;
	}
}

// Called when the game starts or when spawned
void AMoney::BeginPlay()
{
	Super::BeginPlay();
	
	compMesh->SetSimulatePhysics(true);
	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));

	getItemUI = CreateWidget<UIH_GetItemUI>(GetWorld(), getitemUIFactory);
	getItemUI->img_Get->SetBrushFromTexture(coinImage);
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

		value += DeltaTime * 0.01;

		if (value > 1)
		{
			value = 1;
		}

		float currSpeed = FMath::Lerp(startSpeed, targetSpeed, value);

		FVector Vt = (player->GetActorLocation() - GetActorLocation()) * currSpeed * DeltaTime;
		FVector P = GetActorLocation() + Vt;
	
		SetActorLocation(P);

		float distance = FVector::Distance(player->GetActorLocation(), GetActorLocation());

		if (distance < 50)
		{
			GetMoney(minMoney, maxMoney);
		}
	}
}

void AMoney::GetMoney(int32 min, int32 max)
{
	int32 randMoney = FMath::RandRange(min, max);
	player->InvenComp->Money += randMoney;
	getItemUI->txt_ItemName->SetText(FText::FromString(FString::Printf(TEXT("문 x %d"), randMoney)));

	int32 widgetCount = player->MainHUD->ItemGetBox->GetChildrenCount();

	if (widgetCount < 7)
	{
		player->MainHUD->ItemGetBox->AddChild(getItemUI);
	}

	Destroy();
}