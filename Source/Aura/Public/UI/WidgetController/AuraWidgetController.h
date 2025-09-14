// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UAuraAbilityInfo_DA;
class AAuraPlayerController;
class AAuraPlayerState;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

USTRUCT( BlueprintType )
struct FWidgetControllerParams
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TObjectPtr<UAttributeSet> AttributeSet;
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnAttributeChangedSignature, float, NewValue );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnPlayerStatChangedDynamicSignature, const int32, NewValue );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnPlayerLevelChangedDynamicSignature, const int32, NewValue, bool, bLevelUp );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FAbilityInfoSignature, const FAuraAbilityInfo&, AbilityInfo );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FSendSignalFromWCSignature );

/**
 *
 */
UCLASS( BlueprintType, Blueprintable )
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable )
	void SetWidgetControllerParams( const FWidgetControllerParams& WidgetControllerParams );

	virtual void BroadcastInitialValues();

	UPROPERTY( BlueprintAssignable )
	FAbilityInfoSignature AbilityInfoDelegate;

	void BroadcastAbilityInfo();

protected:
	UPROPERTY( EditDefaultsOnly, Category = "Abilities" )
	TObjectPtr<UAuraAbilityInfo_DA> AbilityInfoDataAsset;

	virtual void BindCallbacksToAttributeChanges();

	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<AAuraPlayerController> AuraPlayerController;

	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<AAuraPlayerState> AuraPlayerState;

	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY( BlueprintReadOnly, Category = "WidgetController" )
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	AAuraPlayerController* GetAuraPC();
	AAuraPlayerState* GetAuraPS();
	UAuraAbilitySystemComponent* GetAuraASC();
	UAuraAttributeSet* GetAuraAS();
};
