// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VillageEntrance.generated.h"

UCLASS()
class MOONLITRPG_API AVillageEntrance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVillageEntrance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class USceneComponent* compRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UBoxComponent* compBlockBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UBoxComponent* compBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UUserWidget> UIFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UQuestWarningUI* warningUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UIH_LoadingUI* loadingUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASH_Player* player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULevelSequence* villageSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULevelSequencePlayer* sequencePlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASequencerPlayer* sequencerActor;

	bool enterVillage = false;

	void PlaySequence();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void GoToVillage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void FinishSequence();
};
