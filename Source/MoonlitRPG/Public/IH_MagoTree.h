// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjectBase.h"
#include "IH_MagoTree.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AIH_MagoTree : public AInteractiveObjectBase
{
	GENERATED_BODY()
	
	AIH_MagoTree();

public:
	virtual void BeginPlay() override;
	
	virtual void Interaction() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDangsanWG> WGFactory;

	UPROPERTY(EditAnywhere)
	class UDangsanWG* WG_Dangsan;

	UPROPERTY(EditAnywhere)
	class ASH_Player* Player;
};
