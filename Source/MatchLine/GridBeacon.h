// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GridBeacon.generated.h"

UENUM()
enum EGridType
{
	Longitude,
	Latitude
};


UCLASS()
class MATCHLINE_API AGridBeacon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridBeacon();

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BeaconBox;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DummyRoot;

	UFUNCTION()
	void SetBeconID(int32& ID);

	UFUNCTION()
	void SetBeconType(EGridType gridtype);

	void FireRay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	FVector FireVectorStart;

	FVector FireVectorEnd;

	int32 BeconID;

	FVector BeaconPosition;

	EGridType BeaconCategory;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
