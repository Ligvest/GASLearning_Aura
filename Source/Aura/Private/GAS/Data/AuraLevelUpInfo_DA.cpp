// Dovzhik Tolya

#include "GAS/Data/AuraLevelUpInfo_DA.h"
FAuraLevelUpInfo UAuraLevelUpInfo_DA::FindLevelUpInfoForXp( int32 XpAmount ) const
{
	int Level = 1;

	// LevelUpInfos[0] is just a placeholder
	// LevelUpInfos[1] is for level 1
	// LevelUpInfos[2] is for level 2
	const int MaxLevel = LevelUpInfos.Num() - 1;

	while ( ( LevelUpInfos[Level].LevelUpXpRequirement <= XpAmount ) && ( Level < MaxLevel ) )
	{
		++Level;
	}

	return LevelUpInfos[Level];
}
int32 UAuraLevelUpInfo_DA::FindLevelForXp( int32 XpAmount ) const
{
	int Level = 1;

	// LevelUpInfos[0] is just a placeholder
	// LevelUpInfos[1] is for level 1
	// LevelUpInfos[2] is for level 2
	const int MaxLevel = LevelUpInfos.Num() - 1;

	while ( ( LevelUpInfos[Level].LevelUpXpRequirement <= XpAmount ) && ( Level < MaxLevel ) )
	{
		++Level;
	}

	return Level;
}

FAuraLevelUpInfo UAuraLevelUpInfo_DA::FindLevelUpInfoForLevel( int32 Level ) const
{
	// LevelUpInfos[0] is just a placeholder
	// LevelUpInfos[1] is for level 1
	// LevelUpInfos[2] is for level 2
	const int MaxLevel = LevelUpInfos.Num() - 1;
	if ( Level <= MaxLevel )
	{
		return LevelUpInfos[Level];
	}

	// Incorrect Level
	check( false );
	return FAuraLevelUpInfo{};
}