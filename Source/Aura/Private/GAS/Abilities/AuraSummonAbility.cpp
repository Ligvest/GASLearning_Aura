// Dovzhik Tolya

#include "GAS/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Algo/RandomShuffle.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	TArray<FVector> SpawnLocations;

	AActor* Avatar = GetAvatarActorFromActorInfo();
	const FVector ActorForward = Avatar->GetActorForwardVector();
	const FVector ActorLocation = Avatar->GetActorLocation();
	const float SpreadAngleDelta = SpawnSpreadAngle / ( NumMinions - 1 );

	const FVector LeftOfSpread = ActorForward.RotateAngleAxis( -SpawnSpreadAngle / 2.f, FVector::UpVector );
	FVector SpawnLocation;
	FHitResult Hit;

	for ( int32 i = 0; i < NumMinions; i++ )
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis( SpreadAngleDelta * i, FVector::UpVector );
		const float Distance = FMath::FRandRange( MinSpawnDistance, MaxSpawnDistance );
		const FVector HorizontalSpawnLocation = ActorLocation + Direction * Distance;

		// This is just a fun and performance efficient way to calculate a point on the ground.
		// But in production it could be better to use a solution with UNavigationSystemV1. A little bit less performant but correct and safe
		// Let's assume that the maximum angle that could be before the actor is MaxLandscapeAngle
		// Convert 45 degrees to radians
		constexpr float MaxLandscapeAngle = 45.f * ( PI / 180.f );
		static_assert( 45.f * ( PI / 180.f ) );
		const float MaxSpawnHeight = Distance * FMath::Tan( MaxLandscapeAngle );
		GetWorld()->LineTraceSingleByChannel( Hit, HorizontalSpawnLocation + FVector( 0.f, 0.f, MaxSpawnHeight ), HorizontalSpawnLocation - FVector( 0.f, 0.f, MaxSpawnHeight * 2 ), ECC_Visibility );
		if ( Hit.bBlockingHit )
		{
			SpawnLocation = Hit.ImpactPoint;
		}
		else
		{
			SpawnLocation = HorizontalSpawnLocation;
		}

		// The correct way is to use this function. It will find a correct point on nav mesh up or down from the SpawnLocation
		// FNavLocation ProjectedLocation;
		// if (UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(SpawnLocation, ProjectedLocation))
		// {
		// 	SpawnLocation = ProjectedLocation.Location;
		// }

		SpawnLocations.Add( SpawnLocation );
	}

	Algo::RandomShuffle( SpawnLocations );
	return SpawnLocations;
}
TSubclassOf<APawn> UAuraSummonAbility::GetRandMinionClass()
{
	if ( MinionClasses.Num() <= 0 )
	{
		check( false );
		return nullptr;
	}

	int Index = FMath::RandRange( 0, MinionClasses.Num() - 1 );
	return MinionClasses[Index];
}
