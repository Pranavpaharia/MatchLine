// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchLineSubSystem.h"

void UMatchLineSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	PlayerScore = 0;
}

void UMatchLineSubSystem::AddScoreValue(int scoreValue)
{
	PlayerScore += scoreValue;
	UE_LOG(LogTemp, Warning, TEXT("Player Score: %d"), PlayerScore);
}