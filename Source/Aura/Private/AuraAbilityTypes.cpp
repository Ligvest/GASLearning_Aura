
#include "AuraAbilityTypes.h"

bool FAuraGameplayEffectContext::NetSerialize( FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess )
{
	//!!!!!!!!!!!!!!!! Warning !!!!!!!!!!!!!!!
	// Any changes to this function also need to be done to FGameplayEffectContextNetSerializer to support Iris replication
	//!!!!!!!!!!!!!!!! Warning !!!!!!!!!!!!!!!

	uint16 RepBits = 0;
	// If we are saving then mark all bits which should be saved
	if ( Ar.IsSaving() )
	{
		if ( bReplicateInstigator && Instigator.IsValid() )
		{
			RepBits |= 1 << 0;
		}
		if ( bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if ( AbilityCDO.IsValid() )
		{
			RepBits |= 1 << 2;
		}
		if ( bReplicateSourceObject && SourceObject.IsValid() )
		{
			RepBits |= 1 << 3;
		}
		if ( Actors.Num() > 0 )
		{
			RepBits |= 1 << 4;
		}
		if ( HitResult.IsValid() )
		{
			RepBits |= 1 << 5;
		}
		if ( bHasWorldOrigin )
		{
			RepBits |= 1 << 6;
		}
		if ( bIsBlockedHit )
		{
			RepBits |= 1 << 7;
		}
		if ( bIsCriticalHit )
		{
			RepBits |= 1 << 8;
		}
		if ( bDebuffSucceeded )
		{
			RepBits |= 1 << 9;
		}
		if ( DebuffDamage > 0.f )
		{
			RepBits |= 1 << 10;
		}
		if ( DebuffDuration > 0.f )
		{
			RepBits |= 1 << 11;
		}
		if ( DebuffFrequency > 0.f )
		{
			RepBits |= 1 << 12;
		}
		if ( DamageTypeTag.IsValid() )
		{
			RepBits |= 1 << 13;
		}
		if ( !DeathImpulse.IsNearlyZero() )
		{
			RepBits |= 1 << 14;
		}
		if ( !KnockbackImpulse.IsNearlyZero() )
		{
			RepBits |= 1 << 15;
		}
		if ( bIsRadialDamage )
		{
			RepBits |= 1 << 16;

			if ( RadialDamageInnerRadius > 0.f )
			{
				RepBits |= 1 << 17;
			}
			if ( RadialDamageOuterRadius > 0.f )
			{
				RepBits |= 1 << 18;
			}
			if ( !RadialDamageOrigin.IsZero() )
			{
				RepBits |= 1 << 19;
			}
		}
	}

	// If we are loading then RepBits will be here still 0
	// But in this methods RepBits will be filled on Loading
	Ar.SerializeBits( &RepBits, 20 );

	// Now we are saving/loading info to/out of archive
	if ( RepBits & ( 1 << 0 ) )
	{
		Ar << Instigator;
	}
	if ( RepBits & ( 1 << 1 ) )
	{
		Ar << EffectCauser;
	}
	if ( RepBits & ( 1 << 2 ) )
	{
		Ar << AbilityCDO;
	}
	if ( RepBits & ( 1 << 3 ) )
	{
		Ar << SourceObject;
	}
	if ( RepBits & ( 1 << 4 ) )
	{
		SafeNetSerializeTArray_Default<31>( Ar, Actors );
	}
	if ( RepBits & ( 1 << 5 ) )
	{
		// This is code of Epic but i'll explain it.
		// If we are loading
		if ( Ar.IsLoading() )
		{
			// Then we check if HitResult exists
			if ( !HitResult.IsValid() )
			{
				// If not then create one
				HitResult = TSharedPtr<FHitResult>( new FHitResult() );
			}
		}
		// And fill it
		// Also here we could use typical Ar << *HitResult but it would call Serialize method
		// Serialize would just serialize everything from HitResult byte to byte
		// But instead we can create our own function NetSerialize and optimize it to minimize the amount
		// of data transfered through the net
		HitResult->NetSerialize( Ar, Map, bOutSuccess );
	}
	if ( RepBits & ( 1 << 6 ) )
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if ( RepBits & ( 1 << 7 ) )
	{
		Ar << bIsBlockedHit;
	}
	if ( RepBits & ( 1 << 8 ) )
	{
		Ar << bIsCriticalHit;
	}
	if ( RepBits & ( 1 << 9 ) )
	{
		Ar << bDebuffSucceeded;
	}
	if ( RepBits & ( 1 << 10 ) )
	{
		Ar << DebuffDamage;
	}
	if ( RepBits & ( 1 << 11 ) )
	{
		Ar << DebuffDuration;
	}
	if ( RepBits & ( 1 << 12 ) )
	{
		Ar << DebuffFrequency;
	}
	if ( RepBits & ( 1 << 13 ) )
	{
		Ar << DamageTypeTag;
	}
	if ( RepBits & ( 1 << 14 ) )
	{
		DeathImpulse.NetSerialize( Ar, Map, bOutSuccess );
	}
	if ( RepBits & ( 1 << 15 ) )
	{
		KnockbackImpulse.NetSerialize( Ar, Map, bOutSuccess );
	}
	if ( RepBits & ( 1 << 16 ) )
	{
		Ar << bIsRadialDamage;

		if ( RepBits & ( 1 << 17 ) )
		{
			Ar << RadialDamageInnerRadius;
		}
		if ( RepBits & ( 1 << 18 ) )
		{
			Ar << RadialDamageOuterRadius;
		}
		if ( RepBits & ( 1 << 19 ) )
		{
			RadialDamageOrigin.NetSerialize( Ar, Map, bOutSuccess );
		}
	}

	if ( Ar.IsLoading() )
	{
		AddInstigator( Instigator.Get(), EffectCauser.Get() );  // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
	// Comment how to override the function
}
