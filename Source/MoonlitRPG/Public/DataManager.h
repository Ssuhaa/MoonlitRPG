// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "DataManager.generated.h"

//[ITEMINFO]---------------------------------------------------------------------//
UENUM(BlueprintType)
enum class EItemType : uint8 //아이템 타입 분류 
{
	Consum,
	Outfit,
	Quest,
	Food,
	Etc,
	None,
};
UENUM(BlueprintType)
enum  class EItemgrade : uint8 // 아이템 등급 분류
{
	Common,
	Rare,
	Unique,
	Legendary,
	None,
};
USTRUCT(BlueprintType)
struct FIteminfo : public FTableRowBase //아이템 기본 정보
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //아이템 타입
	EItemType itemType = EItemType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // 아이템 등급
	EItemgrade itemgrade = EItemgrade::Common;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // 아이템 이름
	FString ItemName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // 아이템 설명
	FString itemDescription = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //아이템 2d 이미지
	class UTexture2D* itemImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Empty.T_Empty'"));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // 쌓을 수 있는지 여부
	bool Stackable = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // 구매 가격
	int32 SellPrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // 판매 가격
	int32 PurchasePrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // 소비시 힐량
	int32 HealAmount = 0;

	bool operator==(const FIteminfo& Other) const 
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
struct FItemGradeData : public FTableRowBase // 아이템 등급 별 데이터.
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemgrade Grade;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PlusEXP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PlusPower;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* Slotlmage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* DiscriptionImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* Effect;

};

//[WeaponINFO]---------------------------------------------------------------------//
UENUM(BlueprintType)
enum  class EWeaponType : uint8 //무기 분류
{
	Sword,
	Dagger,
	Bow,
	None,
};

USTRUCT(BlueprintType)
struct FUpGradeMoneyData : public FTableRowBase //업그레이드 시 필요 재화 데이터.
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 UpgradeNeedMoney;
};

USTRUCT(BlueprintType)
struct FUpgradeNeedItem //업그레이드시 필요 재료
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //업그레이드시 필요 재료
	int32 iteminfoIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //필요 수량
	int32 UseAmont = 0;

	bool operator==(const FUpgradeNeedItem& Other) const
	{
		return iteminfoIndex == Other.iteminfoIndex &&
			UseAmont == Other.UseAmont;
	}
};

USTRUCT(BlueprintType)
struct FWeaponNeedItem //업그레이드 카운트 별 필요 재료
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FUpgradeNeedItem> UpgradeNeedItem;// 강화는 최대 3번만 어레이 각각 3개 만들어야함.

	bool operator==(const FWeaponNeedItem& Other) const
	{
		return UpgradeNeedItem == Other.UpgradeNeedItem;
	}
};


USTRUCT(BlueprintType)
struct FWeaponinfo : public FTableRowBase //무기 아이템 기본 정보
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //무기 명
	FString WeaponName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //무기 타입
	EWeaponType WeaponType = EWeaponType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //무기 스태딕메쉬
	TObjectPtr<class UStaticMesh> Mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //돌파에 필요한 재료 리스트
	TArray<FWeaponNeedItem> UpgradeItemList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //공격력 초기값 (38-40, 41-48, 49-60, 61-83 )
	float initPower = 38;

	bool operator==(const FWeaponinfo& Other) const
	{
		return WeaponName == Other.WeaponName && 
			WeaponType == Other.WeaponType &&
			Mesh == Other.Mesh &&
			UpgradeItemList == Other.UpgradeItemList &&
			initPower == Other.initPower;
	}

};


USTRUCT(BlueprintType)
struct FWeaponGradeData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) //현재 레벨
	int32 Level = 1;
	UPROPERTY(BlueprintReadOnly) // 최대 레벨업 가능 레벨
	int32 MaxLevel = 20;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) //현재 EXP
	int32 CurrEXP = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) //현재 Power
	int32 CurrPower = 38;
	UPROPERTY(BlueprintReadOnly) //최대 EXP
	int32 MaxEXP = 600;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) // 몇번 업그레이드 했는지
	int32 UpgradeCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // 착용 여부
	bool isEquip = false;

	bool operator==(const  FWeaponGradeData& Other) const
	{
		return Level == Other.Level &&
			MaxLevel == Other.MaxLevel &&
			CurrEXP == Other.CurrEXP &&
			MaxEXP == Other.MaxEXP &&
			UpgradeCount == Other.UpgradeCount;
	}


};


DECLARE_MULTICAST_DELEGATE(FlevelUpDel);
//[InvenINFO]---------------------------------------------------------------------//
USTRUCT(BlueprintType)
struct FInvenItem : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid InvenID = FGuid::NewGuid();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemInfoIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WeaponInfoIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 itemAmount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponGradeData WeaponData;

	bool operator==(const FInvenItem& Other) const //비교 연산자
	{
		return ItemInfoIndex == Other.ItemInfoIndex &&
			WeaponInfoIndex == Other.WeaponInfoIndex &&
			WeaponData == Other.WeaponData &&
			InvenID == Other.InvenID;
	}


	FlevelUpDel SendLevelUpClear;

public:
	void SetWeaponPower(ADataManager* DataManager);
	bool Upgrade(int32* playerMoney, bool isHaveAllItem, ADataManager* DataManager);//돌파
	void levelUP(FItemGradeData GradeData);//강화
	bool PlusCurrEXP(int32 TotalEXP, int32 TotalAmount, int32* playerMoney, FItemGradeData GradeData);// 다른 무기 아이템 넣기

};

//[DialougueINFO]---------------------------------------------------------------------//
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

//[QuestINFO]---------------------------------------------------------------------//

UENUM(BlueprintType)
enum class EQuestType : uint8 //퀘스트 대 분류
{
	Main,
	Today,
	Sub,
	Total,
	None,
};

UENUM(BlueprintType)
enum class ESubQuestType : uint8 //퀘스트 소 분류
{
	Contact,
	Hunt,
	GetItem,
	None,
};

UENUM(BlueprintType)
enum class EQuestState : uint8 //퀘스트 진행 상황
{
	Continue,
	Done,
	None,
};

USTRUCT(BlueprintType)
struct  FRewarditem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RewardItem = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WeaponData = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Amount = 0;

};

USTRUCT(BlueprintType)
struct  FQuestReward
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRewarditem> RewardItems;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RewardMoney = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RewardEXP = 0;
};


USTRUCT(BlueprintType)
struct FQuestRequirements
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Requirementindex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isRequirements = false;
};


USTRUCT(BlueprintType)
struct FQuestInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestType Type = EQuestType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESubQuestType SubType = ESubQuestType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString QuestName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString LocationName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FQuestRequirements> Requirements;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FQuestReward Reward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DialougueIndex = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestState Queststate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Summary = "None";

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
	FString EnemyPlaceName;
};

USTRUCT(BlueprintType)
struct FinvenData
{
	GENERATED_BODY()
public: 
	UPROPERTY(EditAnywhere)
	FInvenItem invenitem;
	UPROPERTY(EditAnywhere)
	FIteminfo iteminfo;
	UPROPERTY(EditAnywhere)
	FWeaponinfo Weaponinfo;
	UPROPERTY(EditAnywhere)
	FItemGradeData itemGradeData;
	UPROPERTY(EditAnywhere)
	FUpGradeMoneyData UpGradeMoneyData;
};


//[DataManager]---------------------------------------------------------------------//

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

	UPROPERTY()
	class ASH_Player* Player;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, category = Data)
	TArray<FIteminfo> itemList;
	UPROPERTY(VisibleAnywhere, category = Data)
	TArray<FNpcInfo> npcList;
	UPROPERTY(VisibleAnywhere, category = Data)
	TArray<FEnemyManagerInfo> EnemyPlaceList;
	UPROPERTY(VisibleAnywhere, category = Data)
	TArray<FWeaponinfo> WeaponList;
	UPROPERTY(VisibleAnywhere, category = Data)
	TArray<FUpGradeMoneyData> UpgradeMoneyData;
	UPROPERTY(VisibleAnywhere, category = Data)
	TArray<FItemGradeData> ItemGradeData;

	//UPROPERTY()
	TArray<FQuestInfo*> ToTalQuestList;
	void MakeTotalQuestList();

	UPROPERTY(VisibleAnywhere)
	TArray<FQuestInfo> MainQuestList;
	UPROPERTY(VisibleAnywhere)
	TArray<FQuestInfo> TodayQuestList;

	TArray<FInvenItem>* InventoryItemList;

	UPROPERTY(VisibleAnywhere, category = SpawnActor)
	TArray<class ANPCBase*> SpawnNPCs;
	UPROPERTY(VisibleAnywhere, category = SpawnActor)
	TArray<class AIH_EnemyManager*> SpawnEnemyManagers;
	UPROPERTY(VisibleAnywhere, category = SpawnActor)
	TArray<class AInteractiveObjectBase*> SpawnInteractObjects;

	template<typename T>
	TArray<T> LoadTable(FString path); //데이터테이블 불러오는 함수
	template<typename T>
	TArray<T> Parse(FString path, UScriptStruct* pScriptStruct); //CSV파일 Struct 어레이로 반환하는 함수
	template<typename T>
	TArray<T*> GetAllActorOfClass(); //원하는 액터를 찾아 액터클래스 어레이로 반환하는 함수.

	template<typename T>
	T GetInfo(int32 Index, const TArray<T> List);

	FinvenData GetData(FInvenItem invenitem);

	void NavigateTarget(FQuestInfo CurrQuest);

};
