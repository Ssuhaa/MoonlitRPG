// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "DataManager.h"
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
private:

	template<typename T = UUserWidget>
	T* CreateWGClass(FString path);

	void SkeletalMeshFinder(FString path);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UMoveComponent* MoveComp;


	TSubclassOf<class UPlayerMainWG> MainWGFactory;
	
	UPROPERTY()
	class UInputAction* fkey;
	UPROPERTY()
	class UInputAction* altkey;
	void interactionObject();
	void OpenScreenshotUI();
	bool screenShotOpen = false;
	float currTime = 0;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Component)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HP)
	float PlayercurrHP = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HP)
	float PlayerTotalHP = 120.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	int32 AdventureLevel = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	int32 AdventureEXP = 0;

	void DamagedPlayer(float DamageValue);
	void HealPlayer(float HealValue);
	void RevivePlayer();
	void FloatingPlayerDamage();
	void SkillCameraFollow(USceneComponent* attachLoc, FName socketName = NAME_None);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UIH_DieUI* dieUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UIH_LoadingUI* loadingUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UMainDialogueUI* dialogueUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UIH_WarningUI* warningUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UScreenShotUI* screenshotUI;
	UPROPERTY()
	class APlayerController* playerCon;
	UPROPERTY()
	class AIH_WarpPoint* warpPoint = nullptr;
	UPROPERTY(EditAnywhere)
	class AInteractiveObjectBase* InteractiveObjects;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* hitImpact;
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class AIH_DamageActor> damageActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class AIH_DamageActor* damageUI;
	
	EPlayerState ChracterState = Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UPlayerMainWG* MainHUD;

	bool bUIOpen = false;
	bool bFighting = false;

	UPROPERTY(EditDefaultsOnly)
	TArray<class USkeletalMesh*> PlayerMesh;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* EquippedComp1;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* EquippedComp2;

	UPROPERTY(EditAnywhere)
	TArray<class UStaticMesh*> weaponMesh;


	UPROPERTY()
	class ADataManager* DataManager;

	void CompleteQuest(FQuestInfo Questinfo);
		
};