// Fill out your copyright notice in the Description page of Project Settings.


#include "IH_EnemyHPUI.h"
#include <UMG/Public/Components/ProgressBar.h>

void UIH_EnemyHPUI::ReduceHP(float currHP, float maxHP)
{
	float HP = currHP / maxHP;

	enemyHP_ReduceBar->SetPercent(HP);
}

void UIH_EnemyHPUI::UpdateHP(float currHP, float maxHP)
{
	float HP = currHP / maxHP;
	
	enemyHP_Bar->SetPercent(HP);
}