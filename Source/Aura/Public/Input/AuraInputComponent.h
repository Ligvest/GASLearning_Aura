// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	TObjectPtr<UAuraInputConfig> InputConfig;

public:
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions( UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc );
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions( UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc )
{
	check( InputConfig );

	for ( const auto& [InputTag, InputAction] : InputConfig->TagsToInputActions )
	{
		if ( InputAction && InputTag.IsValid() )
		{
			if ( PressedFunc )
			{
				BindAction( InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag );
			}

			if ( ReleasedFunc )
			{
				BindAction( InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag );
			}

			if ( HeldFunc )
			{
				BindAction( InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag );
			}
		}
	}
}
