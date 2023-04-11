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

	UPROPERTY(EditAnywhere)
	class USoundBase* activeSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UIH_WarpActiveUI> warpUIFactory;

	UPROPERTY(EditAnywhere)
	class UIH_WarpActiveUI* warpUI;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* activeEffect;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction() override;		// ����ϴ� ���Ϳ��� �ٲ㾵 �� �ֵ��� �����Լ��� ����.

	UPROPERTY(EditAnywhere)
	bool bsavePoint = false;

	void RemoveUI();
};
