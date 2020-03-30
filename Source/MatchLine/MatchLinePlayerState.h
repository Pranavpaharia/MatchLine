// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MatchLinePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MATCHLINE_API AMatchLinePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 PlayerMoves;
	 
	FORCEINLINE void DecreamentPlayerMoves() { if (PlayerMoves > 0) PlayerMoves--; }

	FORCEINLINE const int32 GetPlayerMoves() { return PlayerMoves; }

	UFUNCTION()
	void SetPlayerMoves(int32 MovesValue);
	
};
