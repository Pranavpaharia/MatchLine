// Fill out your copyright notice in the Description page of Project Settings.


#include "CellBlock.h"
#include "MatchLineGameMode.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACellBlock::ACellBlock()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BlockMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMat;

		

		FConstructorStatics()
			: BlockMesh(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"))
			, BaseMat(TEXT("Material'/Game/StarterContent/Materials/M_Basic_Wall.M_Basic_Wall'"))
		{

		} 
	};

	static FConstructorStatics ConstructorStatics;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBox"));
	CollisionBox->InitBoxExtent(FVector(30, 70, 80));
	CollisionBox->SetSimulatePhysics(true);
	CollisionBox->SetConstraintMode(EDOFMode::YZPlane);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetLinearDamping(1.f);
	CollisionBox->SetAngularDamping(1.f);
	SetRootComponent(CollisionBox);

	//Create Static Mesh Component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMeshComponent"));
	BlockMesh->SetStaticMesh(ConstructorStatics.BlockMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(0.1f,1.3f,1.3f));
	BlockMesh->SetupAttachment(CollisionBox);
	BlockMesh->SetMaterial(0, ConstructorStatics.BaseMat.Get());
	BlockMesh->SetSimulatePhysics(false);
	BlockMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	
	BaseMaterial = ConstructorStatics.BaseMat.Get();
	hightlightColor = FLinearColor::FromSRGBColor(FColor::Emerald);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MomentumCheckDelay = 0.1f;

}

// Called when the game starts or when spawned
void ACellBlock::BeginPlay()
{
	Super::BeginPlay();

	CheckMomentumTimerDelegate.BindUFunction(this, FName("GetBodyMomentum"));

	GetWorldTimerManager().SetTimer(CheckMomentumTimerHandle, CheckMomentumTimerDelegate, MomentumCheckDelay, true, 0.1f);

	SetBoxCategory();

}

// Called every frame
void ACellBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACellBlock::SetBoxCategory()
{
	dynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	const int32 randomInt = FMath::RandRange(1, 2);

	switch (randomInt)
	{
		case 1:
		{
			dynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Red);
			boxColor = FLinearColor::Red;
			break;
		}
		case 2:
		{
			dynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Yellow);
			boxColor = FLinearColor::Yellow;
			break;
		}
		case 3:
		{
			dynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Blue);
			boxColor = FLinearColor::Blue;
			break;
		}
		case 4:
		{
			dynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Green);
			break;
		}
		
	}

	BlockMesh->SetMaterial(0, dynamicMaterial);
	
}



void ACellBlock::SetLatitude(int32& LatID)
{
	vID = LatID;
}

void ACellBlock::SetLongitude(int32& LongID)
{
	hID = LongID;
}

void ACellBlock::UpdateIDsToGameManager()
{
	FVector2D vecID = FVector2D(vID, hID);
	AMatchLineGameMode* gmode = Cast<AMatchLineGameMode>(GetWorld()->GetAuthGameMode());
	if (gmode)
	{
		gmode->AddCellBlockMapEntry(this, vecID);
	}
}

void ACellBlock::PreparetoBlink()
{
	SetBoxCategory();
	FVector NewPosition = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 1200);
	this->TeleportTo(NewPosition, FRotator::ZeroRotator, true, false);
}

void ACellBlock::SethvID(int32& lvID, int32& lhID)
{
	vID = lhID;
	hID = lvID;
}

void ACellBlock::GetBodyMomentum()
{
	MomemtumValue = CollisionBox->GetPhysicsLinearVelocity().Size();

	if (MomemtumValue == 0)
	{
		GetWorldTimerManager().ClearTimer(CheckMomentumTimerHandle);

		AMatchLineGameMode* gmode = Cast<AMatchLineGameMode>(GetWorld()->GetAuthGameMode());
		if (gmode)
		{
			gmode->CellMomentumZero();
		}
	}
	
}

void ACellBlock::StartTimerForMomentumCheck()
{
	GetWorldTimerManager().SetTimer(CheckMomentumTimerHandle, CheckMomentumTimerDelegate, MomentumCheckDelay, true, 0.1f);
	//UE_LOG(LogTemp, Warning, TEXT("Starting Momentum Check Timer"));
}



