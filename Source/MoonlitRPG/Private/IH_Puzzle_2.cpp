// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_Puzzle_2.h"

AIH_Puzzle_2::AIH_Puzzle_2()
{
	for (int32 i = 1; i <= 1; i++)
	{
		meshArr[meshArr.Num()-1]->DestroyComponent();
		meshArr.RemoveAt(meshArr.Num()-1);
	}
}

void AIH_Puzzle_2::BeginPlay()
{
	Super::BeginPlay();
}

void AIH_Puzzle_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}