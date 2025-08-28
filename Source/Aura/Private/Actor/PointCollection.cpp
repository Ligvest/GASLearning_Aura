// Dovzhik Tolya

#include "Actor/PointCollection.h"

#include "GAS/AuraGasBpLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Pt_0 = CreateDefaultSubobject<USceneComponent>( "Pt_0" );
	ImmutablePts.Add( Pt_0 );
	SetRootComponent( Pt_0 );

	Pt_1 = CreateDefaultSubobject<USceneComponent>( "Pt_1" );
	ImmutablePts.Add( Pt_1 );
	Pt_1->SetupAttachment( GetRootComponent() );

	Pt_2 = CreateDefaultSubobject<USceneComponent>( "Pt_2" );
	ImmutablePts.Add( Pt_2 );
	Pt_2->SetupAttachment( GetRootComponent() );

	Pt_3 = CreateDefaultSubobject<USceneComponent>( "Pt_3" );
	ImmutablePts.Add( Pt_3 );
	Pt_3->SetupAttachment( GetRootComponent() );

	Pt_4 = CreateDefaultSubobject<USceneComponent>( "Pt_4" );
	ImmutablePts.Add( Pt_4 );
	Pt_4->SetupAttachment( GetRootComponent() );

	Pt_5 = CreateDefaultSubobject<USceneComponent>( "Pt_5" );
	ImmutablePts.Add( Pt_5 );
	Pt_5->SetupAttachment( GetRootComponent() );

	Pt_6 = CreateDefaultSubobject<USceneComponent>( "Pt_6" );
	ImmutablePts.Add( Pt_6 );
	Pt_6->SetupAttachment( GetRootComponent() );

	Pt_7 = CreateDefaultSubobject<USceneComponent>( "Pt_7" );
	ImmutablePts.Add( Pt_7 );
	Pt_7->SetupAttachment( GetRootComponent() );

	Pt_8 = CreateDefaultSubobject<USceneComponent>( "Pt_8" );
	ImmutablePts.Add( Pt_8 );
	Pt_8->SetupAttachment( GetRootComponent() );

	Pt_9 = CreateDefaultSubobject<USceneComponent>( "Pt_9" );
	ImmutablePts.Add( Pt_9 );
	Pt_9->SetupAttachment( GetRootComponent() );

	Pt_10 = CreateDefaultSubobject<USceneComponent>( "Pt_10" );
	ImmutablePts.Add( Pt_10 );
	Pt_10->SetupAttachment( GetRootComponent() );
}

TArray<USceneComponent*> APointCollection::GetGroundPoints( const FVector& GroundLocation, int32 NumPoints, float YawOverride )
{
	checkf( ImmutablePts.Num() >= NumPoints, TEXT( "Attempted to access ImmutablePts out of bounds." ) );

	TArray<USceneComponent*> ArrayCopy;

	for ( USceneComponent* Pt : ImmutablePts )
	{
		if ( ArrayCopy.Num() == NumPoints ) return ArrayCopy;

		FVector PtLocation = Pt->GetComponentLocation();
		FVector Pt0Location = Pt_0->GetComponentLocation();

		if ( Pt != Pt_0 )
		{
			// Rotate each point around Pt_0
			FVector ToPoint = PtLocation - Pt0Location;
			ToPoint = ToPoint.RotateAngleAxis( YawOverride, FVector::UpVector );
			PtLocation = Pt0Location + ToPoint;
		}

		// Ignore All players
		TArray<AActor*> IgnoreActors;
		UAuraGasBpLibrary::GetCharactersInRadus( this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation() );
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors( IgnoreActors );

		// Line trace from 500 height to the ground location
		FHitResult HitResult;
		const FVector RaisedLocation = FVector( PtLocation.X, PtLocation.Y, PtLocation.Z + 500.f );
		const FVector LoweredLocation = FVector( PtLocation.X, PtLocation.Y, PtLocation.Z - 500.f );
		GetWorld()->LineTraceSingleByProfile( HitResult, RaisedLocation, LoweredLocation, FName( "BlockAll" ), QueryParams );

		const FVector ZAdjustedLocation = FVector( PtLocation.X, PtLocation.Y, HitResult.ImpactPoint.Z );
		Pt->SetWorldLocation( ZAdjustedLocation );
		// ImpactNormal is an orthogonal normal from a trace origin to the hit location
		// MakeRotFromZ makes rotator which makes your object after rotation to have the same Z axis as the HitResult.ImpactNormal
		Pt->SetWorldRotation( UKismetMathLibrary::MakeRotFromZ( HitResult.ImpactNormal ) );

		ArrayCopy.Add( Pt );
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
}
