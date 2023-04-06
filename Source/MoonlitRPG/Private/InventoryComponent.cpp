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
		inputArray.Add(tempAction.Object); //0�� ��
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
	if (Player->bUIOpen == false && !inventory->IsInViewport()) //�κ��丮�� �������� ������
	{
		inventory->AddToViewport(); // ����Ʈ�� ����.
	}
	else // ����������
	{
		inventory->RemoveInventory(); // ����Ʈ���� �����Ѵ�.
	}
}

void UInventoryComponent::CommonCheckSameItemAfterAdd(int32 ItemInfoIndex, int32 Amount) // �Ϲ� ������ �߰� �Լ�
{
	
	int32 value = FindItem(ItemInfoIndex); //������ �ִ��� Ȯ��, ������ �ε����� ��ȯ ������ -1�� ��ȯ.
	FInvenItem currGetItem;
	if (value > -1)
	{
		if (DataManager->GetInfo(ItemInfoIndex, DataManager->itemList)->Stackable) // ������ �ִ��� Ȯ�� �� ���� �� ������ �߰�
		{
			invenItemArr[value].itemAmount += Amount;
		}
		else //�ƴϸ� ���� �߰�
		{
			currGetItem.ItemInfoIndex = ItemInfoIndex;
			currGetItem.itemAmount = Amount;
			currGetItem.InvenID = FGuid::NewGuid();
			invenItemArr.Add(currGetItem);
		}
	}
	else //������ ���� �߰�
	{
		currGetItem.ItemInfoIndex = ItemInfoIndex;
		currGetItem.itemAmount = Amount;
		currGetItem.InvenID = FGuid::NewGuid();
		invenItemArr.Add(currGetItem);
	}
}

void UInventoryComponent::WeaponAddItemToinven(int32 ItemInfoIndex, int32 WeaponInfoIndex) //���� �߰� �Լ�
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
	//�κ��丮�� �ִ� �������� ���� �������� �����ߴ��� ã�Ƽ� �ε����� ��ȯ, ���ٸ� -1�� ��ȯ.
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
	int32 currEquip = FindItem(ChangeItem); //������ ���Ⱑ �κ��丮�� �����ϴ��� Ȯ��
	if (currEquip > -1) //�ش� ����� ���� ��Ŵ.
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
	int32 currEquip = CheckWeaponisEquip(); //������ ���Ⱑ �ִ��� üũ��
	if (currEquip > -1) //�ش� ����� ���� ��Ŵ.
	{
		invenItemArr[currEquip].WeaponData.isEquip = false;
	}

	int32 index = FindItem(ChangeItem); // �ٲ� ���⸦ ã�� �� 
	if (index > -1)
	{
		//�ٲ� ������ ���뿩�θ� ����.
		invenItemArr[index].WeaponData.isEquip = true;
		//�÷��̾� ����������Ʈ�� ������ ����.
		Player->AttackComp->WeaponChange(invenItemArr[index].WeaponInfoIndex);
		FinvenData data = DataManager->GetData(ChangeItem);
		Player->currEquipWeapon = data;
		// ���Ⱑ �� ���Ͽ� ����
		Player->SwitchWeaponPos();
		return true;
	}

	return false;

}



int32 UInventoryComponent::MinusItemAmount(int32 MinusInvenItem, int32 Amount) //���ϴ� ������ŭ �������� ���� ���� ������ ��ȯ
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


int32 UInventoryComponent::GiveAllItem(int32 MinusItem) //��� �������� ���� � ������ ��ȯ
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


int32 UInventoryComponent::FindItem(int32 iteminfoindex) //�������� ã�Ƽ� ��� �ε����� ��ȯ
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