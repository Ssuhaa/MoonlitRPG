// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjectBase.h"
#include "IH_TreasureBox.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AIH_TreasureBox : public AInteractiveObjectBase
{
	GENERATED_BODY()
	
	AIH_TreasureBox();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compTop;
	bool isOpen = false;
	float rotationY = 0;
	float dissolveTop = 1;
	float dissolveBottom = 1;
	float currTime = 0;
	
	UPROPERTY()
	class UMaterialInstanceDynamic* topMat;
	UPROPERTY()
	class UMaterialInstanceDynamic* bottomMat;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* openEffect;

public:
	virtual void Interaction() override;
};
