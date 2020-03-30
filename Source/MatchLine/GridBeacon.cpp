// Fill out your copyright notice in the Description page of Project Settings.


#include "GridBeacon.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerActionInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGridBeacon::AGridBeacon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BlockMesh;

		FConstructorStatics() : BlockMesh(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"))
		{

		}
	};

	static FConstructorStatics ConstructorStatics;

	
	BeaconBox = CreateDefaultSubobject<UBoxComponent>(FName("BeaconBox"));
	BeaconBox->InitBoxExtent(FVector(30,80,80));
	BeaconBox->SetSimulatePhysics(false);
	BeaconBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	BeaconBox->SetCollisionObjectType(ECC_WorldDynamic);
	BeaconBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(BeaconBox);

}

// Called when the game starts or when spawned
void AGridBeacon::BeginPlay()
{
	Super::BeginPlay();

	FireVectorStart = GetActorLocation();
	FireVectorEnd = FVector(FireVectorStart.X,FireVectorStart.Y + 1200,FireVectorStart.Z);
}

// Called every frame
void AGridBeacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridBeacon::FireRay()
{
	FCollisionResponseParams ResponseParam(ECollisionResponse::ECR_Overlap);
	FCollisionQueryParams QueryParams;

	TArray<FHitResult> HitResults;
	GetWorld()->LineTraceMultiByChannel(HitResults, FireVectorStart, FireVectorEnd, ECC_WorldDynamic, QueryParams, ResponseParam);

	for (auto hitResult : HitResults)
	{
		if (hitResult.Actor.IsValid())
		{
			FString strName;
			hitResult.Actor->GetName(strName);
			//UE_LOG(LogTemp, Warning, TEXT("Block Hit with Name: %s "), *strName);

			IPlayerActionInterface* playerInterface = Cast<IPlayerActionInterface>(hitResult.GetActor());

			if (playerInterface)
			{
				switch (BeaconCategory)
				{
					case EGridType::Latitude:
					{
						playerInterface->SetLatitude(BeconID);
						break;
					}

					case EGridType::Longitude:
					{
						playerInterface->SetLongitude(BeconID);
						playerInterface->UpdateIDsToGameManager();
						break;
					}
				}
				
			}
		}
	}
}

void AGridBeacon::SetBeconID(int32& ID)
{
	BeconID = ID;
	//UE_LOG(LogTemp, Warning, TEXT("Beacon ID: %d "), BeconID);
}

void AGridBeacon::SetBeconType(EGridType gridtype)
{
	BeaconCategory = gridtype;

	switch (BeaconCategory)
	{
		case EGridType::Latitude:
		{
			FireVectorStart = GetActorLocation();
			FireVectorEnd = FVector(FireVectorStart.X, FireVectorStart.Y, FireVectorStart.Z - 1200);
			break;
		}
		case EGridType::Longitude:
		{
			FireVectorStart = GetActorLocation();
			FireVectorEnd = FVector(FireVectorStart.X, FireVectorStart.Y + 1200, FireVectorStart.Z);
			break;
		}

	}
}

