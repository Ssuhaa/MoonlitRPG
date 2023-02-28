// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

USTRUCT(BlueprintType)
struct FIteminfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ItemName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString itemDiscription = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* itemImage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemAmont = 0;

};

UCLASS()
class MOONLITRPG_API AItemBase : public AActor
{
	GENERATED_BODY()
		
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UStaticMeshComponent* Mesh;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = iteminfo)
	FIteminfo ItemInfomation;

};
