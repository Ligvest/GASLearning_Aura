// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Container for our gameplay tags.
 * Here we create them and save them so we have convenient access to each tag by variable and not by string literal
 *
 */
struct FAuraGameplayTags
{
public:
	static FAuraGameplayTags& Get() { return GameplayTags; };
	void InitializeNativeGameplayTags();
	FGameplayTag AddGameplayTag( const FString& AttributeNamePrefix, const FString& AttributeName, const FString& AttributeHint );

	// Primary Attributes Tags
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	// Secondary Attributes Tags
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	// Input
	FGameplayTag AuraInput_LBM;
	FGameplayTag AuraInput_RBM;
	FGameplayTag AuraInput_1;
	FGameplayTag AuraInput_2;
	FGameplayTag AuraInput_3;
	FGameplayTag AuraInput_4;

	// Values Tags
	FGameplayTag Values_Damage_Arcane;
	FGameplayTag Values_Damage_Fire;
	FGameplayTag Values_Damage_Lightning;
	FGameplayTag Values_Damage_Physical;

	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Physical;

	TMap<FGameplayTag, FGameplayTag> DamageTypeToDamageRes;

	// Status Effects Tags
	FGameplayTag Effects_HitReact;

private:
	static FAuraGameplayTags GameplayTags;
	const FString SecondaryAttributesPrefix{ "Attributes.Secondary." };
	const FString ResistanceAttributesPrefix{ "Attributes.Resistance." };
	const FString PrimaryAttributesPrefix{ "Attributes.Primary." };
	const FString InputPrefix{ "AuraInput." };
	const FString DamageTypePrefix{ "Values.Damage." };
	const FString EffectsPrefix{ "Effects." };
};
