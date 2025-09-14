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

	// None Tag
	FGameplayTag None;

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
	FGameplayTag AuraInput;
	FGameplayTag AuraInput_LMB;
	FGameplayTag AuraInput_RMB;
	FGameplayTag AuraInput_1;
	FGameplayTag AuraInput_2;
	FGameplayTag AuraInput_3;
	FGameplayTag AuraInput_4;
	FGameplayTag AuraInput_Passive_1;
	FGameplayTag AuraInput_Passive_2;

	// Values Tags
	FGameplayTag Values_Damage_Arcane;
	FGameplayTag Values_Damage_Fire;
	FGameplayTag Values_Damage_Lightning;
	FGameplayTag Values_Damage_Physical;
	FGameplayTag Values_XP;

	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Physical;

	// GameplayCues
	FGameplayTag GameplayCue_FireBlast;

	// Offensive Ability Tags
	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Fire_FireBlast;
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_Arcane_ArcaneShards;

	// Passive Ability Tags
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;
	FGameplayTag Abilities_Passive_ListenForEvents;

	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	// Cooldown Tags
	FGameplayTag Cooldowns_Fire_FireBolt;
	FGameplayTag Cooldowns_Lightning_Electrocute;
	FGameplayTag Cooldowns_Arcane_ArcaneShards;

	// Status Effects Tags
	FGameplayTag Effects_HitReact;

	// Debuffs Tags
	FGameplayTag Debuff;
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;

	// Debuffs Info to Pass it using SetByCallerMagnitude
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;

	// Knockback info
	FGameplayTag Knockback_Chance;
	FGameplayTag Knockback_Impulse;

	// Montage events
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	// Combat Socket Tags
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_Tail;

	// Meta Attribute Tags
	FGameplayTag Attributes_Meta_IncomingXP;

	// Player Block Tags
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;

	TMap<FGameplayTag, FGameplayTag> DamageTypeToDamageRes;
	TMap<FGameplayTag, FGameplayTag> DamageTypeToDebuff;

public:
	const FString SecondaryAttributesPrefix{ "Attributes.Secondary." };
	const FString ResistanceAttributesPrefix{ "Attributes.Resistance." };
	const FString MetaAttributesPrefix{ "Attributes.Meta." };
	const FString PrimaryAttributesPrefix{ "Attributes.Primary." };
	const FString InputPrefix{ "AuraInput." };
	const FString DamageTypePrefix{ "Values.Damage." };
	const FString EffectsPrefix{ "Effects." };
	const FString MontageAttackPrefix{ "Montage.Attack." };
	const FString CombatSocketPrefix{ "CombatSocket." };
	const FString CooldownsPrefix{ "Cooldowns." };
	const FString ValuesPrefix{ "Values." };

	const FString AbilitiesPrefix{ "Abilities." };
	const FString AbilitiesFirePrefix{ "Abilities.Fire." };
	const FString AbilitiesLightningPrefix{ "Abilities.Lightning." };
	const FString AbilitiesArcanePrefix{ "Abilities.Arcane." };
	const FString AbilitiesPassivePrefix{ "Abilities.Passive." };
	const FString StatusPrefix{ "Status." };
	const FString TypePrefix{ "Type." };
	const FString DebuffPrefix{ "Debuff." };
	const FString KnockbackPrefix{ "Knockback." };
	const FString PlayerBlockPrefix{ "Player.Block." };

private:
	static FAuraGameplayTags GameplayTags;
};
