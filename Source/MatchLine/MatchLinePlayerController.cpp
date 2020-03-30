// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MatchLinePlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AMatchLinePlayerController::AMatchLinePlayerController()
{
	
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMatchLinePlayerController::BeginPlay()
{
	Super::BeginPlay();

}