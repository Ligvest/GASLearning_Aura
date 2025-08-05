// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

	UFUNCTION( BlueprintCallable )
	void UpgradeAttribute( const FGameplayTag AttributeTag );

protected:
	UFUNCTION( BlueprintCallable )
	virtual void BindCallbacksToAttributeChanges() override;

	UFUNCTION( BlueprintCallable )
	virtual void BroadcastInitialValues() override;

	UPROPERTY( EditDefaultsOnly )
	TObjectPtr<UAuraAttributeInfoDataAsset> AttributeInfoDataAsset;
};
