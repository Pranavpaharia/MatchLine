#pragma once
#include "UObject/Interface.h"
#include "PlayerActionInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerActionInterface : public UInterface
{
	GENERATED_BODY()
};

class IPlayerActionInterface
{
	GENERATED_BODY()

public:
	/** Add interface function declarations here */
	virtual void MousePressHappened();

	virtual void SetLatitude(int32& LatID);

	virtual void SetLongitude(int32& LongID);

	virtual void UpdateIDsToGameManager();
};