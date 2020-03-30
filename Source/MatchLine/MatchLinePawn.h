// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CellBlock.h"
#include "PlayerActionInterface.h"
#include "MatchLineSubSystem.h"
#include "MatchLinePawn.generated.h"

class AMatchLinePlayerState;

UENUM()
enum EPawnState
{
	FirstCellCaptured,
	SecondCellCaptured,
	None
};

UENUM()
enum ELineState
{
	Horizontal,
	Vertical,
	NoLine
};

UCLASS(config=Game)
class AMatchLinePawn : public APawn,public IPlayerActionInterface
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	

	

protected:
	void OnResetVR();
	
	void OnMouseDownToSelectFirstCube();
	
	ACellBlock* RayCastForCube(FVector& StartPosition, FVector& EndPosition, bool debugHelper);

	void OnMouseReleasedToSelectSecondCube();

	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ACellBlock* FirstCell;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ACellBlock* SecondCell;

	//UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<FVector2D> CellIDList;
	
	EPawnState CurrentPawnState;
	ELineState CurrentLineState;
	TArray<ACellBlock*> LineCubes;

	bool CheckWinningCondition();

	FString PlayerScore;
	FString MoveValue;

	UMatchLineSubSystem* subsystem;

	AMatchLinePlayerState* pState;
};
