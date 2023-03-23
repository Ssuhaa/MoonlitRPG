// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemBase.h"
#include "QuestComponent.generated.h"

UENUM(BlueprintType)
enum class EQuestType :	uint8
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
	FVector ReauireLoc;
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOONLITRPG_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	TArray<class UInputAction*> inputArray;

	void QuestUIOpen();

	TSubclassOf<class UQuestWG> QuestFactory;

	UPROPERTY()
	class ASH_Player* Player;

public:	
	// Called every frame
	virtual void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);

	void reciveQuest(FQuestInfo sendQuest);

	UPROPERTY(EditAnywhere)
	TArray<FQuestInfo> playerQuestList;
	
	UPROPERTY()
	class UQuestWG* QuestWG;
};