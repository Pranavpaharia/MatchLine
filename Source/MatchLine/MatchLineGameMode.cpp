// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MatchLineGameMode.h"
#include "MatchLinePlayerController.h"
#include "MatchLinePlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "MatchLinePawn.h"

AMatchLineGameMode::AMatchLineGameMode() : verticleFloor(0)
{
	
	// Set Default Pawn Class
	DefaultPawnClass = AMatchLinePawn::StaticClass();
	// Set Player Controller Class
	PlayerControllerClass = AMatchLinePlayerController::StaticClass();
	//Set Player State Class
	PlayerStateClass = AMatchLinePlayerState::StaticClass();
	
	TimerDelay = 1.f;
	GridSize = 6;
	CellVolume = 160;
	BeaconVolume = 160;
	CellCounter = 0;
}

void AMatchLineGameMode::BeginPlay()
{
	Super::BeginPlay();

	
	BlockList.Reserve(GridSize*GridSize);
	SpawnGridTimerDelegate.BindUFunction(this, FName("SpawnGridCells"), GridSize);

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, SpawnGridTimerDelegate, TimerDelay,true);

	SpawnGridBeacons();


	PlayerPawn = Cast<AMatchLinePawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerPawn != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Pawn "));
	}

}

void AMatchLineGameMode::SpawnGridCells(const int& Size)
{
	const int TotalCells = Size;

	if (BlockList.Num() >= Size * Size)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}


	for (int32 GridIndex = 0; GridIndex < TotalCells; GridIndex++)
	{
		int32 cellhID = GridIndex % TotalCells;
		const float HorizonOffSet = cellhID * CellVolume;
		
		const FVector CellLocation = FVector(100, HorizonOffSet, 1400);

		ACellBlock* SoloBlock = GetWorld()->SpawnActor<ACellBlock>(CellLocation, FRotator(0,0,0));
		SoloBlock->SethvID(verticleFloor,cellhID);
		
		FVector2D CellPos = FVector2D(cellhID, verticleFloor);
		BlockList.Emplace(SoloBlock);
		CellBlockMap.Add(CellPos, SoloBlock);
		cellMomentumDelegate.AddDynamic(SoloBlock, &ACellBlock::StartTimerForMomentumCheck);
	}

	verticleFloor++;
}

void AMatchLineGameMode::SpawnGridBeacons()
{
	for (int i = 0; i < 6; i++)
	{
		int32 cellhID = i % 6;
		const float VericalOffSet = cellhID * BeaconVolume;

		FVector blockLocation = FVector(100, -200, VericalOffSet+ 80);
		AGridBeacon* beaconBlock = GetWorld()->SpawnActor<AGridBeacon>(blockLocation, FRotator(0, 0, 0));
		beaconBlock->SetBeconID(i);
		beaconBlock->SetBeconType(EGridType::Longitude);
		BeaconVerticalList.Add(beaconBlock);
	}

	for (int i = 0; i < 6; i++)
	{
		int32 cellhID = i % 6;
		const float HorizontalOffSet = cellhID * BeaconVolume;

		FVector blockLocation = FVector(100, HorizontalOffSet, 1050);
		AGridBeacon* beaconBlock = GetWorld()->SpawnActor<AGridBeacon>(blockLocation, FRotator(0, 0, 0));
		beaconBlock->SetBeconID(i);
		beaconBlock->SetBeconType(EGridType::Latitude);
		BeaconHorizontalList.Add(beaconBlock);
	}
	
}

void AMatchLineGameMode::CellMomentumZero()
{
	CellCounter++;

	if (CellCounter == GridSize * GridSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Cells Have Become Stationary Start the Grid Mapping System "));
		CellCounter = 0;
		CellBlockMap.Reset();
		FireBeaconRay();
		PlayerPawn->EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

void AMatchLineGameMode::AddCellBlockMapEntry(ACellBlock* CellBlock, FVector2D& ID)
{
	CellBlockMap.Add(ID, CellBlock);
	FString strName;
	CellBlock->GetName(strName);
	//UE_LOG(LogTemp, Warning, TEXT("Block Name %s:  ID X: %d | Y: %d "), *strName,ID.X,ID.Y);
}

void AMatchLineGameMode::FireBeaconRay()
{
	for (auto& beacon : BeaconHorizontalList)
	{
		beacon->FireRay();
	}

	for (auto& beacon : BeaconVerticalList)
	{
		beacon->FireRay();
	}

}

void AMatchLineGameMode::InitiateMovementCheckBroadCast()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling from Game mode BroadCast"));
	cellMomentumDelegate.Broadcast();
}



