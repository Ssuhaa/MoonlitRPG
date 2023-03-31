// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_Puzzle_3.h"

AIH_Puzzle_3::AIH_Puzzle_3()
{
	for (int32 i = 1; i <= 2; i++)
	{
		meshArr[meshArr.Num()-1]->DestroyComponent();
		meshArr.RemoveAt(meshArr.Num()-1);
	}
}

void AIH_Puzzle_3::BeginPlay()
{
	Super::BeginPlay();
}

void AIH_Puzzle_3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
