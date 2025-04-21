// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraHUDWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnHealthChangedSignature, float, NewHealth );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnMaxHealthChangedSignature, float, NewMaxHealth );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnManaChangedSignature, float, NewMana );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnMaxManaChangedSignature, float, NewMaxMana );

/**
 *
 */
UCLASS()
class AURA_API UAuraHUDWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() const override;

	UPROPERTY( BlueprintAssignable )
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY( BlueprintAssignable )
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY( BlueprintAssignable )
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY( BlueprintAssignable )
	FOnMaxManaChangedSignature OnMaxManaChanged;

protected:
	void HealthChanged( const FOnAttributeChangeData& ChangeData ) const;
	void MaxHealthChanged( const FOnAttributeChangeData& ChangeData ) const;
	void ManaChanged( const FOnAttributeChangeData& ChangeData ) const;
	void MaxManaChanged( const FOnAttributeChangeData& ChangeData ) const;
	// Here we bind callbacks to be called whenever our attributes changes ( damage to Health etc. )
	virtual void BindCallbacksToAttributeChanges() const override;
};
