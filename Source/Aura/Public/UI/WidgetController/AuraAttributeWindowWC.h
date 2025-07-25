// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAttributeWindowWC.generated.h"

// forward declarations
class UAuraAttributeInfoDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnAttributeInfoChanged, const FAuraAttributeInfo&, AttributeInfo );

/**
 *
 */
UCLASS( Blueprintable ) class AURA_API UAuraAttributeWindowWC : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	UPROPERTY( BlueprintAssignable )
	FOnAttributeInfoChanged OnAttributeInfoChanged;

	UPROPERTY( BlueprintAssignable )
	FOnPlayerStatChangedDynamicSignature OnAttributePointsChangedDynamicDelegate;

	UPROPERTY( BlueprintAssignable )
	FOnPlayerStatChangedDynamicSignature OnSpellPointsChangedDynamicDelegate;

protected:
	UFUNCTION( BlueprintCallable )
	virtual void BindCallbacksToAttributeChanges() const override;

	UFUNCTION( BlueprintCallable )
	virtual void BroadcastInitialValues() const override;

	UPROPERTY( EditDefaultsOnly )
	TObjectPtr<UAuraAttributeInfoDataAsset> AttributeInfoDataAsset;
};
