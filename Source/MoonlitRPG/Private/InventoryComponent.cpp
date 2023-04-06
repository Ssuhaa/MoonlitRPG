// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <UMG/Public/Blueprint/UserWidget.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "InventoryWG.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "AttackComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UInputAction> tempAction(TEXT("/Script/EnhancedInput.InputAction'/Game/input/Key_Tab.Key_Tab'"));
	if (tempAction.Succeeded())
	{
		inputArray.Add(tempAction.Object); //0번 탭
	}

	ConstructorHelpers::FClassFinder<UInventoryWG> tempWG(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_WG_Inventory.BP_WG_Inventory_C'"));
	if (tempWG.Succeeded())
	{
		invenFactory = tempWG.Class; 
	}
	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASH_Player>(GetOwner());
	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(),ADataManager::StaticClass()));
	DataManager->InventoryItemList = &invenItemArr;
	inventory = CreateWidget<UInventoryWG>(GetWorld(), invenFactory);
	inventory->InvenComp = this;
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent)
{
	if (inputArray.IsValidIndex(0))
	{
		EnhancedInputComponent->BindAction(inputArray[0], ETriggerEvent::Triggered, this, &UInventoryComponent::InventoryOpen); // Tab
	}
}


void UInventoryComponent::InventoryOpen()
{
	if (Player->bUIOpen == false && !inventory->IsInViewport()) //인벤토리가 열려있지 않으면
	{
		inventory->AddToViewport(); // 뷰포트에 띄운다.
	}
	else // 열려있으면
	{
		inventory->RemoveInventory(); // 뷰포트에서 제거한다.
	}
}

void UInventoryComponent::CommonCheckSameItemAfterAdd(int32 ItemInfoIndex, int32 Amount) // 일반 아이템 추가 함수
{
	
	int32 value = FindItem(ItemInfoIndex); //같은게 있는지 확인, 있으면 인덱스를 반환 없으면 -1을 반환.
	FInvenItem currGetItem;
	if (value > -1)
	{
		if (DataManager->GetInfo(ItemInfoIndex, DataManager->itemList)->Stackable) // 쌓을수 있는지 확인 후 쌓을 수 있으면 추가
		{
			invenItemArr[value].itemAmount += Amount;
		}
		else //아니면 새로 추가
		{
			currGetItem.ItemInfoIndex = ItemInfoIndex;
			currGetItem.itemAmount = Amount;
			currGetItem.InvenID = FGuid::NewGuid();
			invenItemArr.Add(currGetItem);
		}
	}
	else //없으면 새로 추가
	{
		currGetItem.ItemInfoIndex = ItemInfoIndex;
		currGetItem.itemAmount = Amount;
		currGetItem.InvenID = FGuid::NewGuid();
		invenItemArr.Add(currGetItem);
	}
}

void UInventoryComponent::WeaponAddItemToinven(int32 ItemInfoIndex, int32 WeaponInfoIndex) //무기 추가 함수
{
	FInvenItem currGetItem;
	currGetItem.ItemInfoIndex = ItemInfoIndex;
	currGetItem.WeaponInfoIndex = WeaponInfoIndex;
	currGetItem.itemAmount = 1;
	currGetItem.InvenID = FGuid::NewGuid();
	currGetItem.SetWeaponPower(DataManager);
	invenItemArr.Add(currGetItem);
}


int32 UInventoryComponent::CheckWeaponisEquip()
{
	//인벤토리에 있는 아이템중 무기 정보에서 착용했는지 찾아서 인덱스를 반환, 없다면 -1을 반환.
	for (int32 i = 0; i < invenItemArr.Num() ; i++)
	{
		if (invenItemArr[i].WeaponData.isEquip)
		{
			return i;
		}
	}

	return -1;
}

bool UInventoryComponent::WeaponOff(TObjectPtr <FInvenItem> ChangeItem)
{
	int32 currEquip = FindItem(ChangeItem); //착용한 무기가 인벤토리에 존재하는지 확인
	if (currEquip > -1) //해당 무기는 해제 시킴.
	{
		invenItemArr[currEquip].WeaponData.isEquip = false;
		Player->GetMesh()->SetSkeletalMesh(Player->PlayerMesh[3]);

		Player->EquippedComp1->SetStaticMesh(nullptr);
		Player->EquippedComp2->SetStaticMesh(nullptr);

		Player->AttackComp->WeaponChange(-1);
		return true;
	}
	return false;
}

bool UInventoryComponent::WeaponSwitch(TObjectPtr <FInvenItem> ChangeItem)
{	
	int32 currEquip = CheckWeaponisEquip(); //착용한 무기가 있는지 체크후
	if (currEquip > -1) //해당 무기는 해제 시킴.
	{
		invenItemArr[currEquip].WeaponData.isEquip = false;
	}

	int32 index = FindItem(ChangeItem); // 바꿀 무기를 찾은 후 
	if (index > -1)
	{
		//바꿀 무기의 착용여부를 변경.
		invenItemArr[index].WeaponData.isEquip = true;
		//플레이어 어택컴포넌트에 정보를 전달.
		Player->AttackComp->WeaponChange(invenItemArr[index].WeaponInfoIndex);
		FinvenData data = DataManager->GetData(ChangeItem);
		Player->currEquipWeapon = data;
		// 무기가 등 소켓에 부착
		Player->SwitchWeaponPos();
		return true;
	}

	return false;

}



int32 UInventoryComponent::MinusItemAmount(int32 MinusInvenItem, int32 Amount) //원하는 개수만큼 아이템을 빼고 남은 개수를 반환
{
	int32 value = FindItem(MinusInvenItem); 
	if (value > -1)
	{
		invenItemArr[value].itemAmount -= Amount;
		int32 result = invenItemArr[value].itemAmount;
		if (invenItemArr[value].itemAmount < 1)
		{
			invenItemArr.RemoveAt(value);
		}
		return result;
	}
	return -1;
}


int32 UInventoryComponent::GiveAllItem(int32 MinusItem) //모든 아이템을 빼고 몇개 뺐는지 반환
{
	int32 value = FindItem(MinusItem);
	if (value > -1)
	{
		int32 result = invenItemArr[value].itemAmount;
		invenItemArr.RemoveAt(value);
		return result;
	}
	return 0;
}

int32 UInventoryComponent::FindItem(TObjectPtr<FInvenItem> invenitem)
{
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		if (invenItemArr[i] == *invenitem)
		{
			return i;
		}
	}
	return -1;
}


int32 UInventoryComponent::FindItem(int32 iteminfoindex) //아이템을 찾아서 어레이 인덱스를 반환
{
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		if(invenItemArr[i].ItemInfoIndex == iteminfoindex)
		{
			return i;
		}
	}
	return -1;
}

TArray<FInvenItem*> UInventoryComponent::FindAllItems(int32 itemindex)
{
	TArray<FInvenItem*> FindItemArr;
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		if (invenItemArr[i].ItemInfoIndex == itemindex)
		{
			FindItemArr.Add(&invenItemArr[i]);
		}
	}

	return FindItemArr;
}



TArray<FInvenItem*> UInventoryComponent::FindAllItemsType(EItemType type)
{
	TArray<FInvenItem*> FindItemArr;
	if (!invenItemArr.IsEmpty())
	{
		for (int32 i = 0; i < invenItemArr.Num(); i++)
		{
			FinvenData data = DataManager->GetData(&invenItemArr[i]);
			if (data.iteminfo->itemType == type)
			{
				FindItemArr.Add(&invenItemArr[i]);
			}
		}
	}
	return FindItemArr;
}


int32 UInventoryComponent::CountItem()
{
	int32 value = 0;
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		value += invenItemArr[i].itemAmount;
	}
	return value;
}