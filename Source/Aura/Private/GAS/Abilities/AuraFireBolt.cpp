// Dovzhik Tolya

#include "GAS/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription( int32 Level ) const
{
	const int32 Damage = DamageScalableFloat.GetValueAtLevel( Level );
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	return FString::Printf( TEXT(
	                            // Title
	                            "<Title>Fire Bolt</>\n"
	                            // Level
	                            "<Small>Level: </><Level>%d</>\n"
	                            // ManaCost
	                            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
	                            // Cooldown
	                            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	                            // Number of FireBolts
	                            "<Default>Launches %d bolts of fire, "
	                            "exploding on impact and dealing: </>"
	                            // Damage
	                            "<Damage>%d</><Default> fire damage with"
	                            " a chance to burn</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, FMath::Min( Level, NumProjectiles ), Damage );
}

FString UAuraFireBolt::GetNextLevelDescription( int32 Level ) const
{
	const int32 Damage = DamageScalableFloat.GetValueAtLevel( Level );
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	return FString::Printf( TEXT(
	                            // Title
	                            "<Title>Next Level</>\n"
	                            // Level
	                            "<Small>Level: </><Level>%d</>\n"
	                            // ManaCost
	                            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
	                            // Cooldown
	                            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	                            // Number of FireBolts
	                            "<Default>Launches %d bolts of fire, "
	                            "exploding on impact and dealing: </>"
	                            // Damage
	                            "<Damage>%d</><Default> fire damage with"
	                            " a chance to burn</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, FMath::Min( Level, NumProjectiles ), Damage );
}