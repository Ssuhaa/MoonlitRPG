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

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //�����
	EItemgrade itemgrade = EItemgrade::Common;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //����̸� (ȹ���ϴ� �������̶� �����ؾ���)
	FString ItemName = "None";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //����̹��� ((ȹ���ϴ� �������̶� �����ؾ���)
	class UTexture2D* itemImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Empty.T_Empty'"));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //�ʿ� ����
	int32 UseAmont = 0;

};

USTRUCT(BlueprintType)
struct FWeaponNeedItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TArray<FUpgradeNeedItem> UpgradeNeedItem;// ��ȭ�� �ִ� 3���� ��� ���� 3�� ��������.

};

USTRUCT(BlueprintType)
struct FWeaponinfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //���� Ÿ��
	EWeaponType WeaponType = EWeaponType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //������
	EItemgrade itemgrade = EItemgrade::Common;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //���� ���µ�޽�
	TObjectPtr<class UStaticMesh> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // ���Ŀ� �ʿ��� ��� ����Ʈ
	TArray<FWeaponNeedItem> UpgradeItemList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //���ݷ� �ʱⰪ ���� (38-40, 41-48, 49-60, 61-83 )
	float Power = 38;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //������ ������ �����ϴ� ���ݷ� (3 , 5 , 10 ,20)
	int32 PlusPower = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) //��޸��� ������������ �߰��Ǵ� EXP�� (3 , 5 , 10 ,20)
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
	void Upgrade(int32* playerMoney,  bool isHaveAllItem)//����
	{
		if (*playerMoney >= UpGradeMoney)// && isHaveAllItem == true) // ���ļ��� �ʿ�
		{
			*playerMoney -= UpGradeMoney;
			UpgradeCount++;
			MaxLevel += 10;
			UpGradeMoney += 5000;
		}
	}

	void levelUP()//��ȭ
	{
		if (CurrEXP >= MaxEXP)
		{
			Level++;
			MaxEXP += PlusEXP;
			CurrEXP = CurrEXP - MaxEXP;
			Power += PlusPower;

		}
	}

	void PlusCurrEXP(int32 TotalEXP, int32 TotalAmount, int32 playerMoney)// �ٸ� ���� ������ �ֱ�
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
