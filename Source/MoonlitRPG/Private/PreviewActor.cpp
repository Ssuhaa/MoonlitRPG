// Fill out your copyright notice in the Description page of Project Settings.


#include "PreviewActor.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SceneCaptureComponent2D.h>
#include <Engine/StaticMesh.h>
#include <Components/SceneComponent.h>
#include <Engine/TextureRenderTarget2D.h>

// Sets default values
APreviewActor::APreviewActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SetRootComponent(rootComp);

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(rootComp);

	CaptureCam = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureCam"));
	CaptureCam->SetupAttachment(rootComp);
	CaptureCam->SetRelativeLocation(FVector(-350.0, 85.0, 0.0));

	ConstructorHelpers::FObjectFinder <UTextureRenderTarget2D> tempMat(TEXT("/Script/Engine.TextureRenderTarget2D'/Game/UI/InventoryOutfit/PreViewRenderTarget.PreViewRenderTarget'"));
	if (tempMat.Succeeded())
	{
		RenderTarget = tempMat.Object;
	}
	CaptureCam->TextureTarget = RenderTarget;

}

// Called when the game starts or when spawned
void APreviewActor::BeginPlay()
{
	Super::BeginPlay();
	CaptureCam->ShowOnlyActorComponents(this, true);
}

// Called every frame
void APreviewActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PreviewMesh -> AddRelativeRotation(FRotator(0.0,RotattionSpeed,0.0));

}

void APreviewActor::SetPreviewMesh(UStaticMesh* MeshData)
{
	if(MeshData == nullptr) return;
	PreviewMesh->SetStaticMesh(MeshData);
	PreviewMesh->SetMaterial(0, MeshData->GetMaterial(0));
}

