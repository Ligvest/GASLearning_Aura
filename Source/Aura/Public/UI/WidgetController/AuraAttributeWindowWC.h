// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAttributeWindowWC.generated.h"

/**
 *
 */
UCLASS( Blueprintable )
class AURA_API UAuraAttributeWindowWC : public UAuraWidgetController
{
	GENERATED_BODY()
protected:
	virtual void BindCallbacksToAttributeChanges() const override;
};
