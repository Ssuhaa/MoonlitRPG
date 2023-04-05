// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
#include "DataManager.h"
#include "NPCBase.generated.h"


UCLASS()
class MOONLITRPG_API ANPCBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* capsuleComp;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY()
	class ASH_Player* player;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UIH_InteractionUI> interactUIFactory;
	UPROPERTY(EditAnywhere)
	class UIH_InteractionUI* interactionUI;

	UPROPERTY(EditAnywhere)
	class UTexture2D* interactionImg;

public:

	UPROPERTY(EditAnywhere)
	int32 idx = 0;

	UPROPERTY(EditAnywhere)
	FString NPCName = TEXT("Sub");
	UPROPERTY(EditAnywhere)
	int32 DialogueNum = 1;

	TArray<FNPCDialogue> NPCTalk;
	void InteractNPC();
	bool bTalking = false;

	FString MakeCSVPath();

	UPROPERTY(EditAnywhere)
	class UCameraComponent* NPCCamComp;
	
};

