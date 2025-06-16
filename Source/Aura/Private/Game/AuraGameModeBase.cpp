// Dovzhik Tolya

#include "Game/AuraGameModeBase.h"

UAuraCharacterClassInfoDA* AAuraGameModeBase::GetDefaultCharacterInfoDA()
{
	check( DefaultCharacterInfoDA );
	return DefaultCharacterInfoDA;
}