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
	bool Stackable = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SellPrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PurchasePrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HealAmount = 0;

	bool operator==(const FIteminfo& Other) const //비교 연산자
	{
		return itemType == Other.itemType 
		&& itemgrade == Other.itemgrade 
		&& ItemName == Other.ItemName 
		&& itemDescription == Other.itemDescription 
		&& itemImage == Other.itemImage 
		&& Stackable == Other.Stackable 
		&& SellPrice == Other.SellPrice 
		&& PurchasePrice == Other.PurchasePrice 
		&& HealAmount == Other.HealAmount;
	}

};



UCLASS()
class MOONLITRPG_API AItemBase : public AActor
{
	GENERATED_BODY()
		
	// Sets default values for this actor's properties
protected:
	AItemBase();

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
	

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UIH_InteractionUI> interactUIFactory;

	UPROPERTY(EditAnywhere)
	class UIH_InteractionUI* interactionUI;

	virtual void GetItem();
	bool bAddWidget = false;
};