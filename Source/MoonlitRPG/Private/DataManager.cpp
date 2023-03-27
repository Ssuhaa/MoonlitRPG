// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManager.h"

// Sets default values
ADataManager::ADataManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	itemList = LoadTable<FIteminfo>(TEXT("/Script/Engine.DataTable'/Game/TableSample/ItemList.ItemList'"));
	npcList = LoadTable<FNpcInfo>(TEXT("/Script/Engine.DataTable'/Game/TableSample/NpcList.NpcList'"));
	newQuestList = LoadTable<FNewQuestInfo>(TEXT("/Script/Engine.DataTable'/Game/TableSample/NewQuestList.NewQuestList'"));
	for (int32 i = 0; i < newQuestList.Num(); i++)
	{
		(&newQuestList[i])->SetDescription(this);
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


//조심 구조체다
void FNewQuestInfo::SetDescription(ADataManager* DataManager)
{
	switch (subType)
	{
	case EKindOfQuest::Contact:
	
		description = FString::Printf(TEXT("%s 를 만나세요"), *DataManager->npcList[requirementIdx].NPCName);
	
	break;
	case EKindOfQuest::Hunt:
		description = TEXT("해당 지역을 청소하시오.");
		break;
	case EKindOfQuest::GetItem:
	
		description = FString::Printf(TEXT("%s 을(를) %d 수집해요"), *DataManager->itemList[requirementIdx].ItemName, goalCount);
	
	break;
	}
}