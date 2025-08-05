// Dovzhik Tolya

#include "Game/AuraGameModeBase.h"

UAuraCharacterClassInfoDA* AAuraGameModeBase::GetDefaultCharacterInfoDA()
{
	check( DefaultCharacterInfoDA );
	return DefaultCharacterInfoDA;
}
UAuraAbilityInfo_DA* AAuraGameModeBase::GetAbilityInfoDA()
{
	check( AbilityInfoDA );
	return AbilityInfoDA;
}