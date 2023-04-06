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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UNiagaraComponent* Particle;

	UPROPERTY()
	class UNiagaraSystem* PickNS;

	UPROPERTY()
	class ASH_Player* Player;


	UPROPERTY()
	class ADataManager* DataManager;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataIndex)
	int32 iteminfoIndex; 
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UUserWidget>> interactUIFactory;

	UPROPERTY(EditAnywhere)
	class UIH_InteractionUI* interactionUI;

	UPROPERTY()
	class UIH_GetItemUI* getItemUI;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* itemEffect;

// 	UPROPERTY(EditAnywhere)
// 	TArray<UParticleSystem*> particleArr;

	EItemgrade currGrade;

	virtual void GetItem();
	bool bAddWidget = false;
};