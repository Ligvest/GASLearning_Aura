// Dovzhik Tolya

#include "Input/AuraInputConfig.h"

UInputAction* UAuraInputConfig::GetInputActionByGameplayTag( const FGameplayTag GameplayTag )
{
	UInputAction** InputAction = TagsToInputActions.Find( GameplayTag );
	if ( InputAction )
	{
		return *InputAction;
	}
	return nullptr;
}