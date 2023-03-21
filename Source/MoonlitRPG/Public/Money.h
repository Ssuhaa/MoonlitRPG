// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Money.generated.h"

UCLASS()
class MOONLITRPG_API AMoney : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoney();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UIH_GetItemUI> getitemUIFactory;

	UPROPERTY()
	class UIH_GetItemUI* getItemUI;

	UPROPERTY(EditAnywhere)
	int32 minMoney = 50;

	UPROPERTY(EditAnywhere)
	int32 maxMoney = 150;

	UPROPERTY(EditAnywhere)
	class UTexture2D* coinImage;

	UPROPERTY()
	class ASH_Player* player;

	UPROPERTY()
	float startSpeed = 0.1;

	UPROPERTY()
	float targetSpeed = 1000;

	float value = 0;
	float currTime = 0;

	void GetMoney(int32 min, int32 max);
};
