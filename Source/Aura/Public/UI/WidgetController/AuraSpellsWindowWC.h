// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellsWindowWC.generated.h"

/**
 *
 */
UCLASS( Blueprintable )
class AURA_API UAuraSpellsWindowWC : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToAttributeChanges() override;
};
