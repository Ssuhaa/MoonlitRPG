// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "DataManager.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consum,
	Outfit,
	Quest,
	Food,
	Etc,
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
struct FIteminfo : public FTableRowBase
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


USTRUCT(BlueprintType)
struct FNPCDialogue
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FString Speaker;
	UPROPERTY(VisibleAnywhere)
	FString Content;
	UPROPERTY(VisibleAnywhere)
	FString Option1;
	UPROPERTY(VisibleAnywhere)
	int32 Next1;
	UPROPERTY(VisibleAnywhere)
	FString Option2;
	UPROPERTY(VisibleAnywhere)
	int32 Next2;
	UPROPERTY(VisibleAnywhere)
	FString Option3;
	UPROPERTY(VisibleAnywhere)
	int32 Next3;

};



UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Main,
	Today,
	Sub,
	Total,
};

UENUM(BlueprintType)
enum class EKindOfQuest : uint8
{
	Contact,
	Hunt,
	GetItem,
};

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	Continue,
	Done,
};

USTRUCT(BlueprintType)
struct  FQuestReward
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FIteminfo> RewardItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RewardMoney;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RewardEXP;
};

USTRUCT(BlueprintType)
struct FQuestRequirements
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> Requirement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isRequirements = false;
};


USTRUCT(BlueprintType)
struct FQuestInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid QuestID = FGuid::NewGuid();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestType Type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EKindOfQuest KindOf;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString LocationName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FQuestRequirements> Requirements;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FQuestReward Reward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestState Queststate;
};


USTRUCT(BlueprintType)
struct FNewQuestInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EQuestType questType = EQuestType::Main;

	UPROPERTY(EditAnywhere)
	EKindOfQuest subType = EKindOfQuest::Contact;

	UPROPERTY(EditAnywhere)
	int32 requirementIdx;

	UPROPERTY(EditAnywhere)
	int32 dialogueIdx;

	UPROPERTY(EditAnywhere)
	FString description;

	UPROPERTY(EditAnywhere)
	int32 goalCount;

	UPROPERTY(EditAnywhere)
	int32 completeNpcIdx;

	void SetDescription(ADataManager* DataManager);
};

USTRUCT(BlueprintType)
struct FNpcInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString NPCName;
};

USTRUCT(BlueprintType)
struct FEnemyManagerInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString LocationName;
};


UCLASS()
class MOONLITRPG_API ADataManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<FIteminfo> itemList;
	UPROPERTY(EditAnywhere)
	TArray<FNpcInfo> npcList;
	UPROPERTY(EditAnywhere)
	TArray<FNewQuestInfo> newQuestList;



	template<typename T>
	TArray<T> LoadTable(FString path);

	template<typename T>
	TArray<T> Parse(FString path, UScriptStruct* pScriptStruct);
	template<typename T>
	TArray<T*> GetAllActorOfClass();

	FNewQuestInfo GetCurrQuestInfo(int32 mainQuestIdx)
	{
		return newQuestList[mainQuestIdx];
	}

};

