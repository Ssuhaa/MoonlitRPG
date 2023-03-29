// Fill out your copyright notice in the Description page of Project Settings.


#include "BroadLeafTree.h"

ABroadLeafTree::ABroadLeafTree()
{
	compMesh->SetRelativeScale3D(FVector(0.7));
}

void ABroadLeafTree::BeginPlay()
{
	Super::BeginPlay();

	if (treeMeshArr.IsValidIndex(0))
	{
		int32 randIndex = FMath::RandRange(0, treeMeshArr.Num()-1);

		compMesh->SetStaticMesh(treeMeshArr[randIndex]);
	}
}

void ABroadLeafTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}