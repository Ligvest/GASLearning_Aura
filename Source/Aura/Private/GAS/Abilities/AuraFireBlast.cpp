// Dovzhik Tolya

#include "GAS/Abilities/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription( int32 Level ) const
{
	const int32 ScaledDamage = DamageScalableFloat.GetValueAtLevel( Level );
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	return FString::Printf( TEXT(
	                            // Title
	                            "<Title>FIRE BLAST</>\n\n"

	                            // Level
	                            "<Small>Level: </><Level>%d</>\n"
	                            // ManaCost
	                            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
	                            // Cooldown
	                            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	                            // Number of Fire Balls
	                            "<Default>Launches %d </>"
	                            "<Default>fire balls in all directions, each coming back and </>"
	                            "<Default>exploding upon return, causing </>"

	                            // Damage
	                            "<Damage>%d</><Default> radial fire damage with"
	                            " a chance to burn</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage );
}

FString UAuraFireBlast::GetNextLevelDescription( int32 Level ) const
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

	                            // Number of Fire Balls
	                            "<Default>Launches %d </>"
	                            "<Default>fire balls in all directions, each coming back and </>"
	                            "<Default>exploding upon return, causing </>"

	                            // Damage
	                            "<Damage>%d</><Default> radial fire damage with"
	                            " a chance to burn</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage );
}