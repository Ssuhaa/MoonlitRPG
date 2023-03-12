// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreviewActor.generated.h"

UCLASS()
class MOONLITRPG_API APreviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreviewActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* rootComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PreviewMesh;
	UPROPERTY(EditAnywhere)
	class USceneCaptureComponent2D* CaptureCam;
	UPROPERTY(EditAnywhere)
	class UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditAnywhere)
	float RotattionSpeed = 0.5;

public:	
	void SetPreviewMesh(UStaticMesh* MeshData);
};
