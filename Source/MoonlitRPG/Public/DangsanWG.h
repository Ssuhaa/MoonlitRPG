// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataManager.h"
#include "DangsanWG.generated.h"

/**
 * 
 */
UCLASS()
class MOONLITRPG_API UDangsanWG : public UUserWidget
{
	GENERATED_BODY()

	UDangsanWG(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_MagoLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_CurrMago;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_MaxMago;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TB_InvenMago;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* VB_Reward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Give;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Close;



	TSubclassOf<class UMagoRewardListWG> RewardFactory;
	UPROPERTY()
	TArray<class UMagoRewardListWG*> RewardWGs;

	UPROPERTY()
	class ADataManager* DataManager;
	UPROPERTY()
	class ASH_Player* Player;


	FDangsanRewardData* DangSanData;

	int32 MagopieceIndex = 0;

	UPROPERTY()
	class UTexture2D* StanimaIcon= LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Empty.T_Empty'"));
	UPROPERTY()
	class UTexture2D* EXPIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/T_Empty.T_Empty'"));
	UPROPERTY()
	class UTexture2D* MoneyIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/UISource/UI_money.UI_money'"));

	void ButtonBinding();

	UFUNCTION()
	void ClickedGive();
	UFUNCTION()
	void ClickedClose();

public:
	void SetDangSanTree();
	void RemoveWG();

	void CurrInvenMagoUpdate();

};
