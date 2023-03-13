// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponItemBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum  class EWeaponType : uint8
{
	None,
	Sword,
	Dagger,
	Bow,
};

USTRUCT(BlueprintType)
struct FUpgradeNeedItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //재료등급
	EItemgrade itemgrade = EItemgrade::Common;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //재료이름 (획득하는 아이템이랑 동일해야함)
	FString ItemName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //재료이미지 ((획득하는 아이템이랑 동일해야함)
	class UTexture2D* itemImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Empty.T_Empty'"));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //필요 수량
	int32 UseAmont = 0;

};

USTRUCT(BlueprintType)
struct FWeaponNeedItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TArray<FUpgradeNeedItem> UpgradeNeedItem;// 강화는 최대 3번만 어레이 각각 3개 만들어야함.

};

USTRUCT(BlueprintType)
struct FWeaponinfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //무기 타입
	EWeaponType WeaponType = EWeaponType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //무기등급
	EItemgrade itemgrade = EItemgrade::Common;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //무기 스태딕메쉬
	TObjectPtr<class UStaticMesh> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // 돌파에 필요한 재료 리스트
	TArray<FWeaponNeedItem> UpgradeItemList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //공격력 초기값 지정 (38-40, 41-48, 49-60, 61-83 )
	float Power = 38;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //레벨업 했을때 증가하는 공격력 (3 , 5 , 10 ,20)
	int32 PlusPower = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //등급마다 레벨업했을때 추가되는 EXP양 (3 , 5 , 10 ,20)
	int32 PlusEXP = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isEquip = false;


	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 Level = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxLevel = 20;
	UPROPERTY (BlueprintReadOnly)
	int32 CurrEXP = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxEXP = 600;
	UPROPERTY(BlueprintReadOnly)
	int32 UpGradeMoney = 5000; 
	UPROPERTY(BlueprintReadOnly)
	int32 LevelUpMoney = 120;
	UPROPERTY(BlueprintReadOnly)
	int32 UpgradeCount = 0;

public:
	void Upgrade(int32* playerMoney,  bool isHaveAllItem)//돌파
	{
		if (*playerMoney >= UpGradeMoney)// && isHaveAllItem == true) // 추후수정 필요
		{
			*playerMoney -= UpGradeMoney;
			UpgradeCount++;
			MaxLevel += 10;
			UpGradeMoney += 5000;
		}
	}

	void levelUP()//강화
	{
		if (CurrEXP >= MaxEXP)
		{
			Level++;
			MaxEXP += PlusEXP;
			CurrEXP = CurrEXP - MaxEXP;
			Power += PlusPower;

		}
	}

	void PlusCurrEXP(int32 TotalEXP, int32 TotalAmount, int32 playerMoney)// 다른 무기 아이템 넣기
	{
		if (playerMoney > LevelUpMoney * TotalAmount)
		{
			CurrEXP += TotalEXP;
		}
	}

};


UCLASS()
class MOONLITRPG_API AWeaponItemBase : public AItemBase
{
	GENERATED_BODY()

public:

	virtual void GetItem() override;

	FWeaponinfo weaponinfo;
};
