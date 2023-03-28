// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitObjectBase.generated.h"

UCLASS()
class MOONLITRPG_API AHitObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float currTime = 0;
	FVector originSpawnPos;
	FVector	originActorPos;
	
	UPROPERTY(EditAnywhere)
	float AnimationPeriod = 2.0f;	// 애니메이션 주기, 이동 거리 조정

	UPROPERTY(EditAnywhere)
	float AnimationAmplitude = 5.0f;	// 애니메이션 진폭, 속도 조정

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* compMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* compSpawnPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AItemBase>> spawnItems;

	int32 currCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 hitCount;

	bool isHit = false;

	void DropItem();
};
