// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "DataManager.generated.h"

//[ITEMINFO]---------------------------------------------------------------------//
UENUM(BlueprintType)
enum class EItemType : uint8 //������ Ÿ�� �з� 
{
	Consum,
	Outfit,
	Quest,
	Food,
	Etc,
	None,
};
UENUM(BlueprintType)
enum  class EItemgrade : uint8 // ������ ��� �з�
{
	Common,
	Rare,
	Unique,
	Legendary,
	None,
};
USTRUCT(BlueprintType)
struct FIteminfo : public FTableRowBase //������ �⺻ ����
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //������ Ÿ��
	EItemType itemType = EItemType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // ������ ���
	EItemgrade itemgrade = EItemgrade::Common;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // ������ �̸�
	FString ItemName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // ������ ����
	FString itemDescription = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //������ 2d �̹���
	class UTexture2D* itemImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Empty.T_Empty'"));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // ���� �� �ִ��� ����
	bool Stackable = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // ���� ����
	int32 SellPrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // �Ǹ� ����
	int32 PurchasePrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // �Һ�� ����
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
struct FItemGradeData : public FTableRowBase // ������ ��� �� ������.
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
enum  class EWeaponType : uint8 //���� �з�
{
	Sword,
	Dagger,
	Bow,
	None,
};

USTRUCT(BlueprintType)
struct FUpGradeMoneyData : public FTableRowBase //���׷��̵� �� �ʿ� ��ȭ ������.
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 UpgradeNeedMoney;
};

USTRUCT(BlueprintType)
struct FUpgradeNeedItem //���׷��̵�� �ʿ� ���
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //���׷��̵�� �ʿ� ���
	int32 iteminfoIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //�ʿ� ����
	int32 UseAmont = 0;

	bool operator==(const FUpgradeNeedItem& Other) const
	{
		return iteminfoIndex == Other.iteminfoIndex &&
			UseAmont == Other.UseAmont;
	}
};

USTRUCT(BlueprintType)
struct FWeaponNeedItem //���׷��̵� ī��Ʈ �� �ʿ� ���
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FUpgradeNeedItem> UpgradeNeedItem;// ��ȭ�� �ִ� 3���� ��� ���� 3�� ��������.

	bool operator==(const FWeaponNeedItem& Other) const
	{
		return UpgradeNeedItem == Other.UpgradeNeedItem;
	}
};


USTRUCT(BlueprintType)
struct FWeaponinfo : public FTableRowBase //���� ������ �⺻ ����
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //���� ��
	FString WeaponName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //���� Ÿ��
	EWeaponType WeaponType = EWeaponType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //���� ���µ�޽�
	TObjectPtr<class UStaticMesh> Mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //���Ŀ� �ʿ��� ��� ����Ʈ
	TArray<FWeaponNeedItem> UpgradeItemList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //���ݷ� �ʱⰪ (38-40, 41-48, 49-60, 61-83 )
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) //���� ����
	int32 Level = 1;
	UPROPERTY(BlueprintReadOnly) // �ִ� ������ ���� ����
	int32 MaxLevel = 20;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) //���� EXP
	int32 CurrEXP = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) //���� Power
	int32 CurrPower = 38;
	UPROPERTY(BlueprintReadOnly) //�ִ� EXP
	int32 MaxEXP = 600;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) // ��� ���׷��̵� �ߴ���
	int32 UpgradeCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // ���� ����
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

	bool operator==(const FInvenItem& Other) const //�� ������
	{
		return ItemInfoIndex == Other.ItemInfoIndex &&
			WeaponInfoIndex == Other.WeaponInfoIndex &&
			WeaponData == Other.WeaponData &&
			InvenID == Other.InvenID;
	}


	FlevelUpDel SendLevelUpClear;

public:
	void SetWeaponPower(ADataManager* DataManager);
	bool Upgrade(int32* playerMoney, bool isHaveAllItem, ADataManager* DataManager);//����
	void levelUP(FItemGradeData GradeData);//��ȭ
	bool PlusCurrEXP(int32 TotalEXP, int32 TotalAmount, int32* playerMoney, FItemGradeData GradeData);// �ٸ� ���� ������ �ֱ�

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
enum class EQuestType : uint8 //����Ʈ �� �з�
{
	Main,
	Today,
	Sub,
	Total,
	None,
};

UENUM(BlueprintType)
enum class ESubQuestType : uint8 //����Ʈ �� �з�
{
	Contact,
	Hunt,
	GetItem,
	None,
};

UENUM(BlueprintType)
enum class EQuestState : uint8 //����Ʈ ���� ��Ȳ
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
	TArray<T> LoadTable(FString path); //���������̺� �ҷ����� �Լ�
	template<typename T>
	TArray<T> Parse(FString path, UScriptStruct* pScriptStruct); //CSV���� Struct ��̷� ��ȯ�ϴ� �Լ�
	template<typename T>
	TArray<T*> GetAllActorOfClass(); //���ϴ� ���͸� ã�� ����Ŭ���� ��̷� ��ȯ�ϴ� �Լ�.

	template<typename T>
	T GetInfo(int32 Index, const TArray<T> List);

	FinvenData GetData(FInvenItem invenitem);

	void NavigateTarget(FQuestInfo CurrQuest);

};
