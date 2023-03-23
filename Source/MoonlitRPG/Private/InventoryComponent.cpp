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
	inventory = CreateWidget<UInventoryWG>(GetWorld(),invenFactory);
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

void UInventoryComponent::CheckSameItemAfterAdd(FIteminfo iteminfo, int32 Amount) // �Ϲ� ������ �߰� �Լ�
{
	int32 value = FindItem(iteminfo); //������ �ִ��� Ȯ��, ������ �ε����� ��ȯ ������ -1�� ��ȯ.
	if (value > -1)
	{
		if (invenItemArr[value].iteminfomation.Stackable) // ������ �ִ��� Ȯ�� �� ���� �� ������ �߰�
		{
			invenItemArr[value].itemAmount += Amount;
		}
		else //�ƴϸ� ���� �߰�
		{
			AddItemToinven(iteminfo, Amount);
		}
	}
	else //������ ���� �߰�
	{
		AddItemToinven(iteminfo, Amount);
	}
}

void UInventoryComponent::CheckSameItemAfterAdd(FIteminfo iteminfo, FWeaponinfo weaponinfo) 
{
	//���� �������� ��� ������ �����Ƿ� �����߰�.
	AddItemToinven(iteminfo, weaponinfo); 
}

int32 UInventoryComponent::CheckWeaponisEquip()
{
	//�κ��丮�� �ִ� �������� ���� �������� �����ߴ��� ã�Ƽ� �ε����� ��ȯ, ���ٸ� -1�� ��ȯ.
	for (int32 i = 0; i < invenItemArr.Num() ; i++)
	{
		if (invenItemArr[i].weaponinfomaiton.isEquip)
		{
			return i;
		}
	}

	return -1;
}

bool UInventoryComponent::WeaponOff(FInvenItem ChangeItem)
{
	int32 currEquip = FindItem(ChangeItem); //������ ���Ⱑ �κ��丮�� �����ϴ��� Ȯ��
	if (currEquip > -1) //�ش� ����� ���� ��Ŵ.
	{
		invenItemArr[currEquip].weaponinfomaiton.isEquip = false;
		Player->GetMesh()->SetSkeletalMesh(Player->PlayerMesh[0]);
		Player->AttackComp->WeaponChange(EWeaponType::None);
		return true;
	}
	return false;
}

bool UInventoryComponent::WeaponSwitch(FInvenItem ChangeItem)
{	
	int32 currEquip = CheckWeaponisEquip(); //������ ���Ⱑ �ִ��� üũ��
	if (currEquip > -1) //�ش� ����� ���� ��Ŵ.
	{
		invenItemArr[currEquip].weaponinfomaiton.isEquip = false;
	}

	int32 index = FindItem(ChangeItem); // �ٲ� ���⸦ ã�� �� 
	if (index > -1)
	{
		//�ٲ� ������ ���뿩�θ� ����.
		invenItemArr[index].weaponinfomaiton.isEquip = true;
		//�÷��̾� ����������Ʈ�� ������ ����.
		Player->AttackComp->WeaponChange(invenItemArr[index].weaponinfomaiton.WeaponType);
		int32 MeshNum = int32(invenItemArr[index].weaponinfomaiton.WeaponType);
		Player->GetMesh()->SetSkeletalMesh(Player->PlayerMesh[MeshNum]);
		return true;
	}

	return false;

}



void UInventoryComponent::AddItemToinven(FIteminfo Getiteminfo, int32 Amount) //���� �̿��� ��
{
	FInvenItem currGetItem;
	currGetItem.iteminfomation = Getiteminfo;
	currGetItem.itemAmount = Amount;
	currGetItem.InvenID = FGuid::NewGuid();
	invenItemArr.Add(currGetItem);

}

void UInventoryComponent::AddItemToinven(FIteminfo Getiteminfo, FWeaponinfo GetWeaponinfo) //����
{
	FInvenItem currGetItem;
	currGetItem.iteminfomation = Getiteminfo;
	currGetItem.weaponinfomaiton = GetWeaponinfo;
	currGetItem.itemAmount = 1;
	currGetItem.InvenID = FGuid::NewGuid();
	invenItemArr.Add(currGetItem);
}

int32 UInventoryComponent::MinusItemAmount(FInvenItem MinusInvenItem, int32 Amount) //������ �� ��
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


int32 UInventoryComponent::FindItem(FIteminfo iteminfo)
{
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		
		if (invenItemArr[i].iteminfomation == iteminfo)
		{
			return i;
		}
	}
	return -1;
}

int32 UInventoryComponent::FindItem(FInvenItem invenitem)
{
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		if (invenItemArr[i] == invenitem)
		{
			return i;
		}
	}
	return -1;
}

int32 UInventoryComponent::FindItem(FString ItemName)
{
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		if (invenItemArr[i].iteminfomation.ItemName == ItemName)
		{
			return i;
		}
	}

	return -1;
}

TArray<FInvenItem> UInventoryComponent::FindAllItems(FIteminfo iteminfo)
{
	TArray<FInvenItem> FindItemArr;
	for (int32 i = 0; i < invenItemArr.Num(); i++)
	{
		if (invenItemArr[i].iteminfomation == iteminfo)
		{
			FindItemArr.Add(invenItemArr[i]);
		}
	}

	return FindItemArr;
}



TArray<FInvenItem> UInventoryComponent::FindAllItemsType(EItemType type)
{
	TArray<FInvenItem> FindItemArr;
	if (!invenItemArr.IsEmpty())
	{
		for (int32 i = 0; i < invenItemArr.Num(); i++)
		{
			if (invenItemArr[i].iteminfomation.itemType == type)
			{
	
				FindItemArr.Add(invenItemArr[i]);
			
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