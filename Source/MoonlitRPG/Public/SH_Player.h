// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "SH_Player.generated.h"

enum EPlayerState : uint8
{
	Idle,
	Walk,
	Run,
	Attack,
};
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UMoveComponent* MoveComp;


	TSubclassOf<class UPlayerMainWG> MainWGFactory;
	
	UPROPERTY()
	class UInputAction* fkey;
	void interactionObject();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HP)
	float PlayercurrHP = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HP)
	float PlayerTotalHP = 120.0f;
	void DamagedPlayer(float DamageValue);
	void HealPlayer(float HealValue);
	void RevivePlayer();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class USH_PlayerAnim* playerAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class UCameraComponent* CamComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class UAttackComponent* AttackComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class UInventoryComponent* InvenComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class UQuestComponent* QuestComp;


	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UUserWidget>> UIFactory;
	UPROPERTY(EditAnywhere)
	class UIH_DieUI* dieUI;
	UPROPERTY(EditAnywhere)
	class UIH_LoadingUI* loadingUI;
	UPROPERTY(EditAnywhere)
	class UMainDialogueUI* dialogueUI;
	UPROPERTY(EditAnywhere)
	class UIH_WarningUI* warningUI;
	UPROPERTY()
	class APlayerController* playerCon;
	UPROPERTY()
	class AIH_WarpPoint* warpPoint = nullptr;
	UPROPERTY(EditAnywhere)
	class AInteractiveObjectBase* InteractiveObjects;
	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* hitImpact;
	
	EPlayerState ChracterState = Idle;
	UPROPERTY()
	class UPlayerMainWG* MainHUD;

	bool bUIOpen = false;
	bool bFighting = false;

	UPROPERTY(EditDefaultsOnly)
	TArray<class USkeletalMesh*> PlayerMesh;
};
