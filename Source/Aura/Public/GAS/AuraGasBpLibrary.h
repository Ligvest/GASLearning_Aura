// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Data/AuraCharacterClassInfoDA.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraGasBpLibrary.generated.h"

class UAuraHUDWidgetController;
/**
 *
 */
UCLASS()
class AURA_API UAuraGasBpLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintPure )
	static UAuraHUDWidgetController* GetHudWC( const UObject* WorldContext );

	UFUNCTION( BlueprintPure )
	static UAuraAttributeWindowWC* GetAttributeWindowWC( const UObject* WorldContext );

	UFUNCTION( BlueprintPure )
	static UAuraCharacterClassInfoDA* GetCharacterClassInfoDA( const UObject* WorldContext );

private:
	static FWidgetControllerParams GetWidgetControllerParams( const UObject* WorldContext );
};
