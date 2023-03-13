// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjectBase.h"
#include "IH_WarpPoint.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AIH_WarpPoint : public AInteractiveObjectBase
{
	GENERATED_BODY()
	
	AIH_WarpPoint();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction() override;

	UPROPERTY(EditAnywhere)
	bool bsavePoint = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UIH_WarpActiveUI> warpUIFactory;

	UPROPERTY(EditAnywhere)
	class UIH_WarpActiveUI* warpUI;

	void RemoveUI();
};
