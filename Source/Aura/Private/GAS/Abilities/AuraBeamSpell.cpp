// Dovzhik Tolya

#include "GAS/Abilities/AuraBeamSpell.h"

#include "GAS/AuraGasBpLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo( const FHitResult& HitResult )
{
	if ( HitResult.bBlockingHit )
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true );
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if ( CurrentActorInfo )
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>( CurrentActorInfo->AvatarActor );
	}
}
void UAuraBeamSpell::TraceFirstTarget( const FVector& BeamTargetLocation )
{
	check( OwnerCharacter );
	if ( OwnerCharacter->Implements<UCombatInterface>() )
	{
		if ( USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon( OwnerCharacter ) )
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add( OwnerCharacter );
			FHitResult HitResult;
			const FVector SocketLocation = Weapon->GetSocketLocation( FName( "TipSocket" ) );
			UKismetSystemLibrary::SphereTraceSingle( OwnerCharacter, SocketLocation, BeamTargetLocation, 10.f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true );

			if ( HitResult.bBlockingHit )
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}

	if ( ICombatInterface* CombatInterface = Cast<ICombatInterface>( MouseHitActor ) )
	{
		if ( !CombatInterface->GetOnDeathDelegate().IsAlreadyBound( this, &UAuraBeamSpell::PrimaryTargetDied ) )
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic( this, &UAuraBeamSpell::PrimaryTargetDied );
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets( TArray<AActor*>& OutAdditionalTargets )
{
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add( MouseHitActor );

	TArray<AActor*> OverlappingActors;
	UAuraGasBpLibrary::GetLiveOpponentsInRadus( SourceActor, OverlappingActors, ActorsToIgnore, 850.f, MouseHitActor->GetActorLocation() );

	int32 NumAdditionalTargets = FMath::Min( GetAbilityLevel() - 1, MaxNumShockTargets );
	// int32 NumAdditionalTargets = 5;

	UAuraGasBpLibrary::GetClosestTargets( NumAdditionalTargets, OverlappingActors, OutAdditionalTargets, MouseHitActor->GetActorLocation() );

	for ( AActor* Target : OutAdditionalTargets )
	{
		if ( ICombatInterface* CombatInterface = Cast<ICombatInterface>( Target ) )
		{
			if ( !CombatInterface->GetOnDeathDelegate().IsAlreadyBound( this, &UAuraBeamSpell::AdditionalTargetDied ) )
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic( this, &UAuraBeamSpell::AdditionalTargetDied );
			}
		}
	}
}