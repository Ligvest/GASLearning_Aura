// Dovzhik Tolya

#include "GAS/Data/AuraAbilityInfo_DA.h"

FAuraAbilityInfo UAuraAbilityInfo_DA::FindAbilityInfoForTag( const FGameplayTag AbilityTag, bool bLogNotFound ) const
{
	for ( const FAuraAbilityInfo& Info : AbilityInformation )
	{
		if ( Info.AbilityTag == AbilityTag )
		{
			return Info;
		}
	}

	if ( bLogNotFound )
	{
		// UE_LOG(LogAura, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAbilityInfo();
}