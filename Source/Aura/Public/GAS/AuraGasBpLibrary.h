// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION( BlueprintCallable )
	static UAuraHUDWidgetController* GetHudWC( const UObject* WorldContext );

	UFUNCTION( BlueprintCallable )
	static UAuraAttributeWindowWC* GetAttributeWindowWC( const UObject* WorldContext );

private:
	static FWidgetControllerParams GetWidgetControllerParams( const UObject* WorldContext );
};
