// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PlayerActionInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/Actor.h"
#include "CellBlock.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCheckMomentumDelegate);

UCLASS()
class MATCHLINE_API ACellBlock : public AActor, public IPlayerActionInterface
{
	GENERATED_BODY()

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	UPROPERTY()
	UMaterial* BaseMaterial;


	UPROPERTY()
	UMaterialInstanceDynamic* dynamicMaterial;

	UPROPERTY()
	FCheckMomentumDelegate ActivateMomentumDelegate;


public:	
	// Sets default values for this actor's properties
	ACellBlock();

	virtual void SetLatitude(int32& LatID) override;

	virtual void SetLongitude(int32& LongID) override;

	virtual void UpdateIDsToGameManager() override;

	void PreparetoBlink();

	
	UFUNCTION()
	void SethvID(int32& lvID, int32& lhID);

	UFUNCTION()
	void StartTimerForMomentumCheck();

	FORCEINLINE const FVector2D GetHVID() const { return FVector2D(hID, vID); }

	FORCEINLINE const int32 GetvID() const { return vID; }

	FORCEINLINE const int32 GethID() const { return hID; }

	FORCEINLINE const FLinearColor GetColorID() const { return boxColor; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetBoxCategory();

	UFUNCTION()
	void GetBodyMomentum();

	float MomentumCheckDelay;

	float MomemtumValue;

	FLinearColor boxColor;

	FLinearColor hightlightColor;

	FTimerHandle CheckMomentumTimerHandle;

	FTimerDelegate CheckMomentumTimerDelegate;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 hID;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 vID;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
