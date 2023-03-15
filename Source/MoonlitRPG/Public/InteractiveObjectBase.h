// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObjectBase.generated.h"

UCLASS()
class MOONLITRPG_API AInteractiveObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* compSpawnPos;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	class UWidgetComponent* compInteractWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AItemBase>> spawnItems;

	class ASH_Player* player;

	UPROPERTY(EditAnywhere)
	FText InteractName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UIH_InteractionUI> interactUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UIH_InteractionUI* interactionUI;

public:
	virtual void Interaction();
};
