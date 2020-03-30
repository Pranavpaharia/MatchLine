// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MatchLineSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class MATCHLINE_API UMatchLineSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UFUNCTION()
	void AddScoreValue(int scoreValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MatchLine)
	int32 PlayerScore;

};
