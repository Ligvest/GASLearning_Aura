// Dovzhik Tolya

#include "GAS/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription( int32 Level ) const
{
	const int32 ScaledDamage = DamageScalableFloat.GetValueAtLevel( Level );
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	return FString::Printf( TEXT(
	                            // Title
	                            "<Title>ARCANE SHARDS</>\n\n"

	                            // Level
	                            "<Small>Level: </><Level>%d</>\n"
	                            // ManaCost
	                            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
	                            // Cooldown
	                            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	                            // Addition Number of Shock Targets
	                            "<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"

	                            // Damage
	                            "<Damage>%d</><Default> at the shard origins.</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, FMath::Min( Level, MaxNumShards ), ScaledDamage );
}

FString UArcaneShards::GetNextLevelDescription( int32 Level ) const
{
	const int32 ScaledDamage = DamageScalableFloat.GetValueAtLevel( Level );
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );

	return FString::Printf( TEXT(
	                            // Title
	                            "<Title>NEXT LEVEL: </>\n\n"

	                            // Level
	                            "<Small>Level: </><Level>%d</>\n"
	                            // ManaCost
	                            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
	                            // Cooldown
	                            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	                            // Addition Number of Shock Targets
	                            "<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"

	                            // Damage
	                            "<Damage>%d</><Default> at the shard origins.</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, FMath::Min( Level, MaxNumShards ), ScaledDamage );
}
