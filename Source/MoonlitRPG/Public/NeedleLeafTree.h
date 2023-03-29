// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitObjectBase.h"
#include "NeedleLeafTree.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API ANeedleLeafTree : public AHitObjectBase
{
	GENERATED_BODY()
	
public:
	ANeedleLeafTree();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> treeMeshArr;
};
