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

UENUM(BlueprintType)
enum  class EItemgrade : uint8
{
	Common,
	Rare,
	Unique,
	Legendary,
};

UENUM(BlueprintType)
enum  class EWeaponType : uint8
{
	Sword,
	Dagger,
	Bow,
};

USTRUCT(BlueprintType)
struct FIteminfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType itemType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemgrade itemgrade = EItemgrade::Common;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ItemName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString itemDescription = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* itemImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Empty.T_Empty'"));
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Stackalbe = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SellPrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PurchasePrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HealAmount = 0;
};

USTRUCT(BlueprintType)
struct FWeaponinfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Power = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FIteminfo> enhancementItem;

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