// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MatchLinePawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "MatchLineSubSystem.h"
#include "MatchLinePlayerState.h"
#include "Engine/World.h"
#include "MatchLineGameMode.h"
#include "DrawDebugHelpers.h"

AMatchLinePawn::AMatchLinePawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer), CurrentPawnState(EPawnState::None), CurrentLineState(ELineState::NoLine)
{
	LineCubes.Reserve(3);
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	PlayerScore = TEXT("Player Score: ");
	MoveValue = TEXT("Moves Left: ");
}

void AMatchLinePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GEngine != nullptr && subsystem != nullptr && pState != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Yellow, PlayerScore + FString::FromInt(subsystem->PlayerScore));
		GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Orange, MoveValue + FString::FromInt(pState->PlayerMoves));

	}
}

void AMatchLinePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", EInputEvent::IE_Pressed, this, &AMatchLinePawn::OnResetVR);
	PlayerInputComponent->BindAction("MouseClick", EInputEvent::IE_Pressed, this, &AMatchLinePawn::OnMouseDownToSelectFirstCube);
	PlayerInputComponent->BindAction("MouseClick", EInputEvent::IE_Released, this, &AMatchLinePawn::OnMouseReleasedToSelectSecondCube);
}

void AMatchLinePawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	//OutResult.Rotation = FRotator(90.0f, 90.0f, 0.0f);
	//OutResult.Rotation = FRotator(0.0f, 0.0f, 0.0f);
	//OutResult.ProjectionMode = ECameraProjectionMode.Orthographic;
}

void AMatchLinePawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMatchLinePawn::OnMouseReleasedToSelectSecondCube()
{
	if (CurrentPawnState == EPawnState::FirstCellCaptured)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FVector StartVec, DirVec, EndVec;
			PC->DeprojectMousePositionToWorld(StartVec, DirVec);
			EndVec = StartVec + DirVec * 5000;
			

			//Make a RayCast to see which Cube we have hit
			SecondCell = RayCastForCube(StartVec, EndVec, true);

			if (SecondCell != nullptr && FirstCell != nullptr)
			{
				if (FirstCell == SecondCell)
				{
					UE_LOG(LogTemp, Warning, TEXT("Both Cells Are Equal can't perform further"));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Selected Cells are different perform checks..."));
					DisableInput(PC);
					CurrentPawnState = EPawnState::SecondCellCaptured;
					if (CheckWinningCondition())
					{
						//Increase the score
						UMatchLineSubSystem* singleInstance = GetWorld()->GetGameInstance()->GetSubsystem<UMatchLineSubSystem>();
						if (singleInstance != nullptr)
						{
							singleInstance->AddScoreValue(LineCubes.Num());
							UE_LOG(LogTemp, Warning, TEXT("Winning Condition True"));
						}

						//Decrease the Moves
						//AMatchLinePlayerState* pState = Cast<AMatchLinePlayerState>(GetPlayerState());
						if (pState != nullptr)
						{
							pState->DecreamentPlayerMoves();
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Winning Condition False"));
						EnableInput(PC);
					}
				}
			}
		}
	}
}

void AMatchLinePawn::OnMouseDownToSelectFirstCube()
{
	//AMatchLinePlayerState* pState = Cast<AMatchLinePlayerState>(GetPlayerState());
	APlayerController* PC = Cast<APlayerController>(GetController());
	if ((PC != nullptr) && (pState != nullptr) && (pState->GetPlayerMoves() > 0))
	{
		FVector StartVec, DirVec, EndVec;
		PC->DeprojectMousePositionToWorld(StartVec, DirVec);
		EndVec = StartVec + DirVec * 5000;

		//Make a RayCast to see which Cube we have hit
		FirstCell = RayCastForCube(StartVec, EndVec, true);

		if (FirstCell != nullptr)
		{
			CurrentPawnState = EPawnState::FirstCellCaptured;
			CurrentLineState = ELineState::NoLine;
		}
	}
}

ACellBlock* AMatchLinePawn::RayCastForCube(FVector& StartPosition, FVector& EndPosition, bool debugHelper)
{
	ACellBlock* targetblock = nullptr;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartPosition, EndPosition, ECC_WorldDynamic);

	if (debugHelper)
	{
		DrawDebugLine(GetWorld(), StartPosition, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}

	if (HitResult.Actor.IsValid())
	{

		if (HitResult.Actor->GetClass()->ImplementsInterface(UPlayerActionInterface::StaticClass()))
		{

		}

		ACellBlock* HitBlock = Cast<ACellBlock>(HitResult.Actor.Get());
		if (HitBlock != nullptr)
		{
			targetblock = HitBlock;
		}
	}
	return targetblock;
}



bool AMatchLinePawn::CheckWinningCondition()
{
	bool bWin = false;
	
	AMatchLineGameMode* gamemode = Cast<AMatchLineGameMode>(GetWorld()->GetAuthGameMode());
	check(gamemode)

	//Check if Line Could be Vertical or Horizontal
	if (FirstCell->GethID() == SecondCell->GethID())
	{
		CurrentLineState = ELineState::Horizontal;
	}
	else if (FirstCell->GetvID() == SecondCell->GetvID())
	{
		CurrentLineState = ELineState::Vertical;
	}
	
	if (CurrentLineState == ELineState::NoLine)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line is neither verticle or Horizontal. Close the Compare "));
		CurrentLineState = ELineState::NoLine;
		CurrentPawnState = EPawnState::None;
		return bWin;
	}
	const TEnumAsByte<ELineState> LineEnumAsByte = CurrentLineState;
	FString EnumAsString = UEnum::GetValueAsString(LineEnumAsByte.GetValue());
	
	switch (CurrentLineState)
	{
		case ELineState::Horizontal:
		{
			FVector2D PosID = FirstCell->GetHVID();
			int32 StartY = FirstCell->GetvID();
			int32 EndY = SecondCell->GetvID();

			if (StartY > EndY)
			{
				auto temp = StartY;
				StartY = EndY;
				EndY = temp;
			}

			const int32 DifHorizontal = (EndY - StartY) + 1;

			//Create and Store Cell ID list of all blocks 
			LineCubes.Reset();
			LineCubes.Reserve(StartY + DifHorizontal);

			for (int i = StartY; i < StartY + DifHorizontal; i++)
			{
				const int xID = PosID.X;
				const int yID = i;
				const FVector2D keyVector = FVector2D(yID, xID);
				if (gamemode->CellBlockMap.Contains(keyVector))
				{
					ACellBlock* NewFresh = gamemode->CellBlockMap[keyVector];
					LineCubes.Add(NewFresh);
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Line has CellObjects of total %d"), LineCubes.Num());

			// If the Line Blocks are Less than 3 Units return false condition
			if (LineCubes.Num() < 3)
				return bWin;
			

			// If the Line Blocks are of different Colors return false condition
			FLinearColor sampleID = LineCubes[0]->GetColorID();
			for (auto Cell : LineCubes)
			{
				if (sampleID != Cell->GetColorID())
					return bWin;
				
			}

			// If you reach here means all good, Start the block work
			for (auto Cell : LineCubes)
			{
				Cell->PreparetoBlink();
			}
			gamemode->InitiateMovementCheckBroadCast();
			bWin = true;
			break;
		}

		case ELineState::Vertical:
		{
			FVector2D PosID = FirstCell->GetHVID();
			int32 StartX = FirstCell->GethID();
			int32 EndX = SecondCell->GethID();

			if (StartX > EndX)
			{
				auto temp = StartX;
				StartX = EndX;
				EndX = temp;
			}


			const int32 DifVertical = EndX - StartX + 1;
			
			//Create and Store Cell ID List of all the selected blocks
			LineCubes.Reset();
			LineCubes.Reserve(StartX + DifVertical);
			for (int i = StartX; i < StartX + DifVertical; i++)
			{
				const int xID = i;
				const int yID = PosID.Y;
				const FVector2D keyVector = FVector2D(yID, xID);
				if (gamemode->CellBlockMap.Contains(keyVector))
				{
					ACellBlock* NewFresh = gamemode->CellBlockMap[keyVector];
					LineCubes.Add(NewFresh);
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Line has CellObjects of total %d"), LineCubes.Num());


			// If the Line Blocks are Less than 3 Units return false condition
			if (LineCubes.Num() < 3)
				return bWin;
			

			// If the Line Blocks are of different Colors return false condition
			FLinearColor sampleID = LineCubes[0]->GetColorID();
			for (auto Cell : LineCubes)
			{
				if (sampleID != Cell->GetColorID())
					return bWin;
			}

			// If you reach here means all good, Start the block work
			for (auto Cell : LineCubes)
			{
				Cell->PreparetoBlink();
			}
			gamemode->InitiateMovementCheckBroadCast();
			bWin = true;

			break;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT(" Line State: %s"), *EnumAsString);
	
	return bWin;
}

void AMatchLinePawn::BeginPlay()
{
	Super::BeginPlay();

	pState = Cast<AMatchLinePlayerState>(GetPlayerState());
	if (pState != nullptr)
	{
		pState->SetPlayerMoves(10);
		UE_LOG(LogTemp, Warning, TEXT(" Player Moves Set to 10"));
	}

	subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMatchLineSubSystem>();

	DisableInput(GetWorld()->GetFirstPlayerController());
}