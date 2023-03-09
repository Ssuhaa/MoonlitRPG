// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetActorBase.h"
#include <UMG/Public/Components/WidgetComponent.h>

// Sets default values
AWidgetActorBase::AWidgetActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
}

// Called when the game starts or when spawned
void AWidgetActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWidgetActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

