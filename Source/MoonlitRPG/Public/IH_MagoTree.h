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
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interaction() override;

	UFUNCTION()
	void CanOffering(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CantOffering(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDangsanWG> WGFactory;

	UPROPERTY(EditAnywhere)
	class UDangsanWG* WG_Dangsan;

	UPROPERTY(EditAnywhere)
	class ASH_Player* Player;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;
};
