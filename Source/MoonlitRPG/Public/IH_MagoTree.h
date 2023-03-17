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

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainDialogueUI> dialogueUIFactory;

	UPROPERTY(EditAnywhere)
	class UMainDialogueUI* dialogueUI;
};
