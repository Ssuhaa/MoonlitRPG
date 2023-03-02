// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consum,
	Outfit,
	Quest,
	Food,
	Count,
};

USTRUCT(BlueprintType)
struct FIteminfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ItemName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString itemDescription = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* itemImage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType itemType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Stackalbe = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SellPrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PurchasePrice = 0;
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

	class ASH_Player* Player;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = iteminfo)
	FIteminfo ItemInformation;

	void GetItem();
};
