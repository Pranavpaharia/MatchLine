// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MatchLinePlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class AMatchLinePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMatchLinePlayerController();

	virtual void BeginPlay() override;

};


