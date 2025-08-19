// Dovzhik Tolya

#include "GAS/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"

void UAuraDamageGameplayAbility::CauseDamage( AActor* TargetActor )
{
	int AbilityLevel = GetAbilityLevel();
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec( DamageEffectClass, AbilityLevel );

	float DamageValue = DamageScalableFloat.GetValueAtLevel( AbilityLevel );
	// Pass DamageValue to PostGameplayEffectExecute and label it with DamageTypeTag
	DamageSpecHandle.Data->SetSetByCallerMagnitude( DamageTypeTag, DamageValue );

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( TargetActor );
	SourceASC->ApplyGameplayEffectSpecToTarget( *DamageSpecHandle.Data, TargetASC );
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults( AActor* TargetActor ) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( TargetActor );
	Params.AbilityLevel = GetAbilityLevel();
	Params.BaseDamage = DamageScalableFloat.GetValueAtLevel( Params.AbilityLevel );
	Params.DamageTypeTag = DamageTypeTag;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackChance = KnockbackChance;
	Params.KnockbackMagnitude = KnockbackMagnitude;
	if ( IsValid( TargetActor ) )
	{
		FRotator Rotation = ( TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation() ).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackImpulse = ToTarget * KnockbackMagnitude;
	}
	return Params;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return DamageScalableFloat.GetValueAtLevel( GetAbilityLevel() );
}