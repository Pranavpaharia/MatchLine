// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CellBlock.h"
#include "GridBeacon.h"
#include "GameFramework/GameModeBase.h"
#include "MatchLineGameMode.generated.h"


class AMatchLinePawn;

/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class AMatchLineGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMatchLineGameMode();
	virtual void BeginPlay() override;
	FORCEINLINE int32 GetSize() const { return GridSize; }
	TMap <FVector2D,ACellBlock*> CellBlockMap;
	//TMap <ACellBlock , FVector2D > BlockMap;
	void CellMomentumZero();

	void AddCellBlockMapEntry(ACellBlock* CellBlock, FVector2D& ID);

	UPROPERTY()
	FCheckMomentumDelegate cellMomentumDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> Menu;
	
	//UMatchLineWidget* MenuWidget;
	UUserWidget* MenuWidget;
	//TSharedRef<ACellBlock> CellBlock(new ACellBlock());

	UFUNCTION()
	void InitiateMovementCheckBroadCast();

private:
	UFUNCTION()
	void SpawnGridCells(const int& Size);

	UFUNCTION()
	void SpawnGridBeacons();

	UFUNCTION()
	void FireBeaconRay();
	
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 GridSize;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 verticleFloor;
	int32 BeaconBase;

	TArray<ACellBlock*> BlockList;
	TArray<AGridBeacon*> BeaconVerticalList;
	TArray<AGridBeacon*> BeaconHorizontalList;
	
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CellVolume;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 BeaconVolume;

	FTimerHandle SpawnTimerHandle;
	FTimerDelegate SpawnGridTimerDelegate;

	float TimerDelay;

	int32 CellCounter;

	AMatchLinePawn* PlayerPawn;

};



