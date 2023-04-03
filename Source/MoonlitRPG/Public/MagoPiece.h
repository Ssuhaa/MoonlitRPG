// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "MagoPiece.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API AMagoPiece : public AItemBase
{
	GENERATED_BODY()

	AMagoPiece();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	float rotateZ = 0;

	UPROPERTY(EditAnywhere)
	TArray<UParticleSystem*> magoEffect;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMago;

	virtual void GetItem() override;

};
