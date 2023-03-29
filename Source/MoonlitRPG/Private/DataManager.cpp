// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManager.h"

// Sets default values
ADataManager::ADataManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	itemList = LoadTable<FIteminfo>(TEXT("/Script/Engine.DataTable'/Game/TableSample/ItemList.ItemList'"));
	WeaponList = LoadTable<FWeaponinfo>(TEXT("/Script/Engine.DataTable'/Game/TableSample/WeaponList.WeaponList'"));
	npcList = LoadTable<FNpcInfo>(TEXT("/Script/Engine.DataTable'/Game/TableSample/NpcList.NpcList'"));
	EnemyPlaceList = LoadTable<FEnemyManagerInfo>(TEXT("/Script/Engine.DataTable'/Game/TableSample/EnemyManagerList.EnemyManagerList'"));
	UpgradeMoneyData = LoadTable<FUpGradeMoneyData>(TEXT("/Script/Engine.DataTable'/Game/TableSample/UpGradeMoneyData.UpGradeMoneyData'"));
	ItemGradeData = LoadTable<FItemGradeData>(TEXT("/Script/Engine.DataTable'/Game/TableSample/ItemGradeData.ItemGradeData'"));
	MainQuestList = LoadTable<FQuestInfo>(TEXT("/Script/Engine.DataTable'/Game/TableSample/MainQuestList.MainQuestList'"));
	
	for (int32 i = 0; i < MainQuestList.Num(); i++)
	{
		(&MainQuestList[i])->SetDescription(this);
	}

}

// Called when the game starts or when spawned
void ADataManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

template<typename T>
TArray<T>  ADataManager::LoadTable(FString path)
{
	TArray<T> data;
	ConstructorHelpers::FObjectFinder<UDataTable> tempTable(*path);
	if (tempTable.Succeeded())
	{
		UDataTable* table = tempTable.Object;
		TArray<FName> names = table->GetRowNames();
		for (int32 i = 0; i < names.Num(); i++)
		{
			T* pInfo = table->FindRow<T>(names[i], TEXT(""));
			data.Add(*pInfo);
		}
	}
	return data;
}


template<typename T>
TArray<T> ADataManager::Parse(FString path, UScriptStruct* pScriptStruct)
{
	TArray<T> Data;

	FString DialogueFile;
	if (FFileHelper::LoadFileToString(DialogueFile, *path))
	{
		TArray<FString> rows;
		DialogueFile.ParseIntoArrayLines(rows);

		TArray<FString> ContentsName;
		rows[0].ParseIntoArray(ContentsName, TEXT(","));

		for (int32 i = 1; i < rows.Num(); i++)
		{
			TArray<FString> Columns;
			rows[i].ParseIntoArray(Columns, TEXT(","));

			T info;
			for (int32 j = 0; j < Columns.Num(); j++)
			{
				FProperty* pro = pScriptStruct->FindPropertyByName(*ContentsName[j]);
				if (pro->IsA<FStrProperty>())
				{
					FString* DataPtr = pro->ContainerPtrToValuePtr<FString>(&info);
					*DataPtr = Columns[j];
				}
				else if (pro->IsA<FIntProperty>())
				{
					int32* DataPtr = pro->ContainerPtrToValuePtr<int32>(&info);
					*DataPtr = FCString::Atoi(Columns[j]);
				}
				else if (pro->IsA<FBoolProperty>())
				{
					bool* DataPtr = pro->ContainerPtrToValuePtr<bool>(&info);
					*DataPtr = Columns[j].ToBool();
				}
				else if (pro->IsA<FFloatProperty>())
				{
					float* DataPtr = pro->ContainerPtrToValuePtr<float>(&info);
					*DataPtr = FString::Atof(*Columns[j]);
				}
			}
			Data.Add(info);
		}
	}

	return Data;
}

template<typename T>
TArray<T*> ADataManager::GetAllActorOfClass()
{
	TArray<class AActor*> Actors;
	TArray<T*> Managers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), T::StaticClass(), Actors);
	for (int32 i = 0; i < Actors.Num(); i++)
	{
		T* CurrManager = Cast<T>(Actors[i]);
		if (CurrManager != nullptr)
		{
			Managers.Add(CurrManager);
		}
	}

	return Managers;
}

FinvenData ADataManager::GetData(FInvenItem invenitem)
{
	FinvenData invenData;
	invenData.invenitem = invenitem;
	invenData.iteminfo = GetInfo(invenitem.ItemInfoIndex, itemList);
	invenData.Weaponinfo = GetInfo(invenitem.WeaponInfoIndex, WeaponList);
	invenData.itemGradeData = GetInfo(int32(invenData.iteminfo.itemgrade), ItemGradeData);
	invenData.UpGradeMoneyData = GetInfo(invenData.invenitem.WeaponData.UpgradeCount, UpgradeMoneyData);

	return invenData;
}

template<typename T>
T ADataManager::GetInfo(int32 Index, const TArray<T> List)
{
	if(Index < 0 || List.IsEmpty())
	{
		T null;
		return null;
	}
	return List[Index];
}

template<typename T>
T ADataManager::GetInfo(int32 index, EDataList DataList)
{	
	switch (DataList)
	{
	default:
		break;
	}
}

//---------<조심> 구조체함수--------------------------------------------------------

void FInvenItem::SetWeaponPower(ADataManager* DataManager)
{
	WeaponData.CurrPower = DataManager->GetInfo(WeaponInfoIndex, DataManager->WeaponList).initPower;
}

bool FInvenItem::PlusCurrEXP(int32 TotalEXP, int32 TotalAmount, int32* playerMoney, FItemGradeData GradeData)
{
	if (*playerMoney >= 120 * TotalAmount)
	{
		*playerMoney -= 120 * TotalAmount;
		*playerMoney = FMath::Clamp(*playerMoney, 0, *playerMoney);
		WeaponData.CurrEXP += TotalEXP;
		levelUP(GradeData);
		return true;
	}
	SendLevelUpClear.Broadcast();
	return false;
}

void  FInvenItem::levelUP(FItemGradeData GradeData)
{
	if (WeaponData.CurrEXP >= WeaponData.MaxEXP)
	{
		WeaponData.Level++;
		WeaponData.MaxEXP += GradeData.PlusEXP;
		WeaponData.CurrEXP = WeaponData.CurrEXP - WeaponData.MaxEXP;
		WeaponData.CurrPower += GradeData.PlusPower;
	}
	SendLevelUpClear.Broadcast();
}

bool FInvenItem::Upgrade(int32* playerMoney, bool isHaveAllItem, ADataManager* DataManager)
{
	int32 UpgradeMoney = DataManager->UpgradeMoneyData[WeaponData.UpgradeCount].UpgradeNeedMoney;
	if (*playerMoney >= UpgradeMoney && isHaveAllItem == true)
	{
		*playerMoney -= UpgradeMoney;
		*playerMoney = FMath::Clamp(*playerMoney, 0, *playerMoney);
		WeaponData.UpgradeCount++;
		WeaponData.MaxLevel += 10;
		return true;
	}
	return false;
}

void FQuestInfo::SetDescription(ADataManager* DataManager)
{
	int32 index = Requirements[0].Requirementindex;
	switch (SubType)
	{
	case ESubQuestType::Contact:
	
		Summary = FString::Printf(TEXT("%s 를 만나자."), *DataManager->GetInfo(index, DataManager->npcList).NPCName);
	break;
	case  ESubQuestType::Hunt:
		Summary = FString::Printf(TEXT("%s 을/를 소탕하라."), *DataManager->GetInfo(index, DataManager->EnemyPlaceList).EnemyPlaceName);
		break;
	case  ESubQuestType::GetItem:
		Summary = FString::Printf(TEXT("%s 을(를) %d 수집하자."), *DataManager->GetInfo(index, DataManager->itemList).ItemName, 1);
	break;
	}
}