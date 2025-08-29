// Dovzhik Tolya

#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "Components/AudioComponent.h"
#include "GAS/AuraGasBpLibrary.h"

void AAuraFireBall::PlayImpactEffects() const
{
	/*
	 * Can't use checks here as this function is called in Destroyed method
	 * And Destroyed method is called during compilation in BP when the engine tries to
	 * remove all actors of this type from world and create new ones if necessary
	check( ImpactEffect );
	check( ImpactSound );
	*/

	// Not necessary to use the gameplay cue here but for learning purposes and for striving to use GAS why not?
	if ( GetOwner() )
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated( GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParams );
	}

	if ( IsValid( FlySoundComponent ) )
	{
		FlySoundComponent->Stop();
		FlySoundComponent->DestroyComponent();
	}
}
void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AAuraFireBall::OnCollisionSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                              const FHitResult& SweepResult )
{
	if ( !IsValidOverlap( OtherActor ) )
	{
		return;
	}

	if ( HasAuthority() )
	{
		if ( UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( OtherActor ) )
		{
			const FVector DeathImpulse = GetActorForwardVector() * ImpactEffectParams.DeathImpulseMagnitude;
			ImpactEffectParams.DeathImpulse = DeathImpulse;
			ImpactEffectParams.TargetASC = TargetASC;
			UAuraGasBpLibrary::ApplyDamageEffect( ImpactEffectParams );
		}
	}
}