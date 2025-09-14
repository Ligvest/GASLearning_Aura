// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraHUDWidgetController.generated.h"

USTRUCT( BlueprintType )
struct FEffectMessageRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FGameplayTag EffectTag;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FText EffectMessage;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSubclassOf<UAuraUserWidget> EffectWidgetClass;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	UTexture2D* EffectImage;
};

// FWD
class UAuraAbilityInfo_DA;
class UAuraAbilitySystemComponent;

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FEffectMessageRowDelegate, const FEffectMessageRow&, TagMessageRow );

/**
 *
 */
UCLASS( Blueprintable )
class AURA_API UAuraHUDWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;

	UPROPERTY( BlueprintAssignable )
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY( BlueprintAssignable )
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY( BlueprintAssignable )
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY( BlueprintAssignable )
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY( BlueprintAssignable )
	FOnAttributeChangedSignature OnXpPercentageChangedDelegate;

	UPROPERTY( BlueprintAssignable )
	FOnPlayerLevelChangedDynamicSignature OnPlayerLevelChangedDynamicDelegate;

	UPROPERTY( BlueprintAssignable )
	FEffectMessageRowDelegate EffectMessageRowDelegate;

	UPROPERTY( BlueprintAssignable )
	FSendSignalFromWCSignature ClearSpellGlobesDelegate;

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Tag message" )
	TObjectPtr<UDataTable> EffectMessageTable;

	// Callbacks
	void OnXpChanged( int32 NewXP ) const;

	void OnEffectWithTagsApplied( const FGameplayTagContainer& TagContainer ) const;
	// Here we bind callbacks to be called whenever our attributes changes ( damage to Health etc. )
	virtual void BindCallbacksToAttributeChanges() override;

	void OnSpellEquipped();

private:
	template <typename T>
	T* GetDataTableRowByTag( UDataTable* DataTable, const FGameplayTag& Tag ) const;
};
template <typename T>
T* UAuraHUDWidgetController::GetDataTableRowByTag( UDataTable* DataTable, const FGameplayTag& Tag ) const
{
	return DataTable->FindRow<T>( Tag.GetTagName(), FString() );
}
