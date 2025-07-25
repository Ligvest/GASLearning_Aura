// Dovzhik Tolya

#include "UI/WidgetController/AuraHUDWidgetController.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"
#include "GAS/Data/AuraAbilityInfo_DA.h"
#include "GAS/Data/AuraLevelUpInfo_DA.h"
#include "Player/AuraPlayerState.h"

void UAuraHUDWidgetController::BroadcastInitialValues() const
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );
	OnHealthChanged.Broadcast( AuraAttributeSet->GetHealth() );
	OnMaxHealthChanged.Broadcast( AuraAttributeSet->GetMaxHealth() );
	OnManaChanged.Broadcast( AuraAttributeSet->GetMana() );
	OnMaxManaChanged.Broadcast( AuraAttributeSet->GetMaxMana() );

	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>( PlayerState );
	OnXpChanged( AuraPS->GetXP() );
}

void UAuraHUDWidgetController::BindCallbacksToAttributeChanges() const
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	// AuraASC should always be a base class for ASC for this project
	check( AuraASC );

	// Bind callback to call it when Health attribute changes
	const FGameplayAttribute& HealthAttribute = AuraAttributeSet->GetHealthAttribute();
	auto& OnHealthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttribute );
	OnHealthChangeDelegate.AddLambda( [this]( const FOnAttributeChangeData& ChangeData ) { OnHealthChanged.Broadcast( ChangeData.NewValue ); } );

	// Bind callback to call it when MaxHealth attribute changes
	const FGameplayAttribute& MaxHealthAttribute = AuraAttributeSet->GetMaxHealthAttribute();
	auto& OnMaxHealthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( MaxHealthAttribute );
	OnMaxHealthChangeDelegate.AddLambda( [this]( const FOnAttributeChangeData& ChangeData ) { OnMaxHealthChanged.Broadcast( ChangeData.NewValue ); } );

	// Bind callback to call it when Mana attribute changes
	const FGameplayAttribute& ManaAttribute = AuraAttributeSet->GetManaAttribute();
	auto& OnManaChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( ManaAttribute );
	OnManaChangeDelegate.AddLambda( [this]( const FOnAttributeChangeData& ChangeData ) { OnManaChanged.Broadcast( ChangeData.NewValue ); } );

	// Bind callback to call it when MaxMana attribute changes
	const FGameplayAttribute& MaxManaAttribute = AuraAttributeSet->GetMaxManaAttribute();
	auto& OnMaxManaChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( MaxManaAttribute );
	OnMaxManaChangeDelegate.AddLambda( [this]( const FOnAttributeChangeData& ChangeData ) { OnMaxManaChanged.Broadcast( ChangeData.NewValue ); } );

	// Bind OnEffectWithTagsApplied to call when an effect with tags is applied
	AuraASC->OnEffectWithTagsAppliedDelegate.AddUObject( this, &UAuraHUDWidgetController::OnEffectWithTagsApplied );

	// PS Broadcasts to every client. But here we subscribe only to our own PSs OnPlayerStatChangedDelegate
	// Thats why on LevelUp or other broadcast only 1 OnPlayerStatChangedDynamicDelegate is broadcasted
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>( PlayerState );
	AuraPS->OnXpChangedDelegate.AddUObject( this, &UAuraHUDWidgetController::OnXpChanged );
	AuraPS->OnLevelChangedDelegate.AddLambda( [this]( int32 NewValue ) { OnPlayerLevelChangedDynamicDelegate.Broadcast( NewValue ); } );

	if ( AuraASC->bStartupAbilitiesGranted )
	{
		OnAbilitiesGranted( AuraASC );
	}
	else
	{
		// TODO: Should I do it in "else"?
		AuraASC->OnAbilitiesGrantedDelegate.AddUObject( this, &UAuraHUDWidgetController::OnAbilitiesGranted );
	}
}

void UAuraHUDWidgetController::OnXpChanged( int32 NewXP ) const
{
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>( PlayerState );
	const int CurrentLevel = AuraPS->LevelUpInfo_DA->FindLevelForXp( NewXP );
	const int PreviousLevel = CurrentLevel - 1;

	// The level can't be less than 1 and we have a placeholder for level 0 in LevelUpInfo so we don't need to check it
	const int PrevLevelMaxXP = AuraPS->LevelUpInfo_DA->FindLevelUpInfoForLevel( PreviousLevel ).LevelUpXpRequirement;
	const int CurrentLevelMaxXP = AuraPS->LevelUpInfo_DA->FindLevelUpInfoForLevel( CurrentLevel ).LevelUpXpRequirement;

	const float XpAmountInProgressBar = static_cast<float>( NewXP - PrevLevelMaxXP );
	const float XpAmountToFillProgressBar = static_cast<float>( CurrentLevelMaxXP - PrevLevelMaxXP );

	const float XpBarPercent = XpAmountInProgressBar / XpAmountToFillProgressBar;

	OnXpPercentageChangedDelegate.Broadcast( XpBarPercent );
}

void UAuraHUDWidgetController::OnEffectWithTagsApplied( const FGameplayTagContainer& TagContainer ) const
{
	FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag( FName( "Message" ) );
	for ( auto Tag : TagContainer )
	{
		if ( Tag.MatchesTag( MessageTag ) )
		{
			FEffectMessageRow* EffectMessageRow = GetDataTableRowByTag<FEffectMessageRow>( EffectMessageTable, Tag );
			EffectMessageRowDelegate.Broadcast( *EffectMessageRow );
		}
	}
}

void UAuraHUDWidgetController::OnAbilitiesGranted( UAuraAbilitySystemComponent* AuraASC ) const
{
	// TODO Get information about all given abilities, look up their Ability Info, and broadcast it to widgets.
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
	    [this, AuraASC]( const FGameplayAbilitySpec& AbilitySpec )
	    {
		    // TODO need a way to figure out the ability tag for a given ability spec.
		    FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec( AbilitySpec );
		    FAuraAbilityInfo AbilityInfo = AbilityInfoDataAsset->FindAbilityInfoForTag( AbilityTag );
		    AbilityInfo.InputTag = AuraASC->GetInputTagFromSpec( AbilitySpec );
		    AbilityInfoDelegate.Broadcast( AbilityInfo );
	    } );

	AuraASC->ForEachAbility( BroadcastDelegate );
}
