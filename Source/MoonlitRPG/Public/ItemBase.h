// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataManager.h"
#include "ItemBase.generated.h"




UCLASS()
class MOONLITRPG_API AItemBase : public AActor
{
	GENERATED_BODY()
		
	// Sets default values for this actor's properties
protected:
	AItemBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UStaticMeshComponent* Mesh;

	class ASH_Player* Player;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = iteminfo)
	FIteminfo ItemInformation;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UUserWidget>> interactUIFactory;

// 	UPROPERTY(EditAnywhere)
// 	TSubclassOf<class UIH_InteractionUI> interactUIFactory;

	UPROPERTY(EditAnywhere)
	class UIH_InteractionUI* interactionUI;

// 	UPROPERTY(EditAnywhere)
// 	TSubclassOf<class UIH_GetItemUI> getitemUIFactory;

	UPROPERTY()
	class UIH_GetItemUI* getItemUI;

	virtual void GetItem();
	bool bAddWidget = false;
};