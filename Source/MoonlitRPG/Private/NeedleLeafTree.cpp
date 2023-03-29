// Fill out your copyright notice in the Description page of Project Settings.


#include "NeedleLeafTree.h"

ANeedleLeafTree::ANeedleLeafTree()
{

}

void ANeedleLeafTree::BeginPlay()
{
	Super::BeginPlay();

	if (treeMeshArr.IsValidIndex(0))
	{
		int32 randIndex = FMath::RandRange(0, treeMeshArr.Num()-1);

		compMesh->SetStaticMesh(treeMeshArr[randIndex]);
	}
}

void ANeedleLeafTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}