// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionInterface.h"

void IPlayerActionInterface::MousePressHappened()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Interface Calling!"));
}

void IPlayerActionInterface::SetLatitude(int32& LatID)
{
	UE_LOG(LogTemp, Warning, TEXT("Base Interface SetLatitude Calling!"));
}

void IPlayerActionInterface::SetLongitude(int32& LongID)
{
	UE_LOG(LogTemp, Warning, TEXT("Base Interface SetLongitude Calling!"));
}

void IPlayerActionInterface::UpdateIDsToGameManager()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Interface UpdateIDsToGameManager Calling!"));
}