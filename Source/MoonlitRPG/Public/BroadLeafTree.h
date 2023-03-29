// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitObjectBase.h"
#include "BroadLeafTree.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API ABroadLeafTree : public AHitObjectBase
{
	GENERATED_BODY()
	
	ABroadLeafTree();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> treeMeshArr;
};
