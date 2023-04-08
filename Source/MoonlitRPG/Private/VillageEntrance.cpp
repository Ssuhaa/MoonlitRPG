// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageEntrance.h"
#include <Components/BoxComponent.h>
#include "QuestWarningUI.h"
#include "SH_Player.h"
#include <Kismet/GameplayStatics.h>
#include "QuestComponent.h"
#include <LevelSequence/Public/LevelSequence.h>
#include "SequencerPlayer.h"
#include "IH_LoadingUI.h"
#include <LevelSequence/Public/LevelSequencePlayer.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerMainWG.h"

// Sets default values
AVillageEntrance::AVillageEntrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(compRoot);

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Village Entrance Component"));
	compBox->SetupAttachment(compRoot);

	compBlockBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Blocking Box"));
	compBlockBox->SetupAttachment(compRoot);
	compBlockBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	ConstructorHelpers::FClassFinder<UUserWidget>tempWarning(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WG_QuestWarning.WG_QuestWarning_C'"));
	if (tempWarning.Succeeded())
	{
		UIFactory = tempWarning.Class;
	}
}

// Called when the game starts or when spawned
void AVillageEntrance::BeginPlay()
{
	Super::BeginPlay();
	
	ALevelSequenceActor* levelActor = nullptr;
	sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), villageSequence, FMovieSceneSequencePlaybackSettings(),levelActor);

	compBox->OnComponentBeginOverlap.AddDynamic(this, &AVillageEntrance::GoToVillage);
	sequencePlayer->OnFinished.AddDynamic(this, &AVillageEntrance::FinishSequence);
	
	player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	sequencerActor = Cast<ASequencerPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ASequencerPlayer::StaticClass()));
	warningUI = CreateWidget<UQuestWarningUI>(GetWorld(), UIFactory);
}

// Called every frame
void AVillageEntrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVillageEntrance::GoToVillage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == player)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), player->QuestComp->MainQuestIDX);

		if (player->QuestComp->MainQuestIDX >= 2)
		{
			player->loadingUI->AddToViewport();
			compBlockBox->DestroyComponent();
			player->GetCharacterMovement()->DisableMovement();

			FTimerHandle timer;
			GetWorld()->GetTimerManager().SetTimer(timer, this, &AVillageEntrance::PlaySequence, 3.0f, false);
		}
		else
		{
			warningUI->AddToViewport();
		}
	}
}

void AVillageEntrance::PlaySequence()
{
	sequencerActor->SetActorHiddenInGame(false);
	player->SetActorHiddenInGame(true);
	player->MainHUD->SetVisibility(ESlateVisibility::Hidden);
	sequencePlayer->Play();
}

void AVillageEntrance::FinishSequence()
{
	sequencerActor->Destroy();
	player->SetActorHiddenInGame(false);
	player->SetActorLocation(FVector(-67580, -41283, 228));
	player->MainHUD->SetVisibility(ESlateVisibility::Visible);
	Destroy();
}