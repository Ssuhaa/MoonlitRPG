// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "SH_Player.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API ASH_Player : public ACharacterBase
{
	GENERATED_BODY()

	ASH_Player();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoveComponent)
	class UMoveComponent* MoveComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class UCameraComponent* CamComp;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
