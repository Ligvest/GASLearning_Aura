// Dovzhik Tolya

#include "GAS/Abilities/Electrocute.h"

FString UElectrocute::GetDescription( int32 Level ) const
{
	const int32 ScaledDamage = DamageScalableFloat.GetValueAtLevel( Level );
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	return FString::Printf( TEXT(
	                            // Title
	                            "<Title>ELECTROCUTE</>\n\n"

	                            // Level
	                            "<Small>Level: </><Level>%d</>\n"
	                            // ManaCost
	                            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
	                            // Cooldown
	                            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	                            // Addition Number of Shock Targets
	                            "<Default>Emits a beam of lightning, "
	                            "propagating to %d additional targets nearby, causing </>"

	                            // Damage
	                            "<Damage>%d</><Default> lightning damage with"
	                            " a chance to stun</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, FMath::Min( Level, MaxNumShockTargets - 1 ), ScaledDamage );
}

FString UElectrocute::GetNextLevelDescription( int32 Level ) const
{
	const int32 ScaledDamage = DamageScalableFloat.GetValueAtLevel( Level );
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	return FString::Printf( TEXT(
	                            // Title
	                            "<Title>NEXT LEVEL:</>\n\n"

	                            // Level
	                            "<Small>Level: </><Level>%d</>\n"
	                            // ManaCost
	                            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
	                            // Cooldown
	                            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	                            // Addition Number of Shock Targets
	                            "<Default>Emits a beam of lightning, "
	                            "propagating to %d additional targets nearby, causing </>"

	                            // Damage
	                            "<Damage>%d</><Default> lightning damage with"
	                            " a chance to stun</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, FMath::Min( Level, MaxNumShockTargets - 1 ), ScaledDamage );
}
