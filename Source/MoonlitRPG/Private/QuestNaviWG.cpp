// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestNaviWG.h"
#include <Kismet/GameplayStatics.h>
#include "SH_Player.h"
#include "QuestNaviActor.h"
#include <UMG/Public/Components/TextBlock.h>

void UQuestNaviWG::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SetDistance();

}

void UQuestNaviWG::SetDistance()
{

	ASH_Player* Player = Cast<ASH_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Player::StaticClass()));
	FVector playerPos = Player->GetActorLocation();
	FVector NaviPos = NaviActor->GetActorLocation();
	int32 dis = (FVector::Distance(NaviPos, playerPos)  - 100 )/10;
	dis = FMath::Clamp(dis, 1, dis);
	TB_Distance-> SetText(FText::FromString(FString::Printf(TEXT("%dm"), dis)));
}
