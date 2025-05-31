// Dovzhik Tolya

#include "GAS/Data/AuraAttributeInfoDataAsset.h"

FAuraAttributeInfo UAuraAttributeInfoDataAsset::GetAttributeInfoByTag( const FGameplayTag& Tag )
{
	for ( const auto& AttributeInfo : AttributeInfos )
	{
		if ( AttributeInfo.Tag.MatchesTagExact( Tag ) )
		{
			return AttributeInfo;
		}
	}

	ensureMsgf( false, TEXT( "Missing attribute?" ) );
	return FAuraAttributeInfo{};
}