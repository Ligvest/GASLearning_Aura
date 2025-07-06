// Dovzhik Tolya

#include "GAS/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage( AActor* TargetActor )
{
	int AbilityLevel = GetAbilityLevel();
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec( DamageEffectClass, AbilityLevel );

	float DamageValue = 0.f;
	for ( const auto& [DamageTypeTag, DamageScalableFloat] : DamageTypeTagToScalableFloat )
	{
		DamageValue = DamageScalableFloat.GetValueAtLevel( AbilityLevel );
		// Pass DamageValue to PostGameplayEffectExecute and label it with DamageTypeTag
		DamageSpecHandle.Data->SetSetByCallerMagnitude( DamageTypeTag, DamageValue );
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( TargetActor );
	SourceASC->ApplyGameplayEffectSpecToTarget( *DamageSpecHandle.Data, TargetASC );
}