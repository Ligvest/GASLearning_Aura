// Dovzhik Tolya

#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// None tag
	None = UGameplayTagsManager::Get().AddNativeGameplayTag( "None", "Tag alternative to nullptr" );

	// Input
	AuraInput = AddGameplayTag( FString{}, "AuraInput", "AuraInput category tag" );
	AuraInput_LMB = AddGameplayTag( InputPrefix, "LMB", "Left Mouse Button" );
	AuraInput_RMB = AddGameplayTag( InputPrefix, "RMB", "Right Mouse Button" );
	AuraInput_1 = AddGameplayTag( InputPrefix, "1", "Key \"1\"" );
	AuraInput_2 = AddGameplayTag( InputPrefix, "2", "Key \"2\"" );
	AuraInput_3 = AddGameplayTag( InputPrefix, "3", "Key \"3\"" );
	AuraInput_4 = AddGameplayTag( InputPrefix, "4", "Key \"4\"" );
	AuraInput_Passive_1 = AddGameplayTag( InputPrefix, "Passive.1", "Passive (meta) tag for equipped passive ability 1" );
	AuraInput_Passive_2 = AddGameplayTag( InputPrefix, "Passive.2", "Passive (meta) tag for equipped passive ability 2" );

	// Effects Tags
	Effects_HitReact = AddGameplayTag( EffectsPrefix, "HitReact", "Tag granted to react to hit" );

	// Primary Attributes Tags
	Attributes_Primary_Strength = AddGameplayTag( PrimaryAttributesPrefix, "Strength", "+PhysicalDamage" );
	Attributes_Primary_Intelligence = AddGameplayTag( PrimaryAttributesPrefix, "Intelligence", "+MagicalDamage, +ManaRegeneration" );
	Attributes_Primary_Resilience = AddGameplayTag( PrimaryAttributesPrefix, "Resilience", "+Armor, +ArmorPenetration" );
	Attributes_Primary_Vigor = AddGameplayTag( PrimaryAttributesPrefix, "Vigor", "+MaxHealth, +HealthRegen" );

	// Secondary Attributes Tags
	Attributes_Secondary_Armor = AddGameplayTag( SecondaryAttributesPrefix, "Armor", "-Damage taken, +Block chance" );
	Attributes_Secondary_ArmorPenetration = AddGameplayTag( SecondaryAttributesPrefix, "ArmorPenetration", "-Enemy armor, +Crit chance" );
	Attributes_Secondary_BlockChance = AddGameplayTag( SecondaryAttributesPrefix, "BlockChance", "+Chance to half incoming damage" );
	Attributes_Secondary_CriticalHitChance = AddGameplayTag( SecondaryAttributesPrefix, "CriticalHitChance", "+Chance to crit" );
	Attributes_Secondary_CriticalHitDamage = AddGameplayTag( SecondaryAttributesPrefix, "CriticalHitDamage", "+Crit damage" );
	Attributes_Secondary_CriticalHitResistance = AddGameplayTag( SecondaryAttributesPrefix, "CriticalHitResistance", "-Crit chance on taken damage" );
	Attributes_Secondary_HealthRegeneration = AddGameplayTag( SecondaryAttributesPrefix, "HealthRegeneration", "+Health regen per second" );
	Attributes_Secondary_ManaRegeneration = AddGameplayTag( SecondaryAttributesPrefix, "ManaRegeneration", "+Mana regen per 1 sec" );
	Attributes_Secondary_MaxHealth = AddGameplayTag( SecondaryAttributesPrefix, "MaxHealth", "+Max Health" );
	Attributes_Secondary_MaxMana = AddGameplayTag( SecondaryAttributesPrefix, "MaxMana", "+Max Mana" );

	// Resistance Tags
	Attributes_Resistance_Arcane = AddGameplayTag( ResistanceAttributesPrefix, "Arcane", "ValueTag: Arcane Resistance" );
	Attributes_Resistance_Fire = AddGameplayTag( ResistanceAttributesPrefix, "Fire", "ValueTag: Fire Resistance" );
	Attributes_Resistance_Lightning = AddGameplayTag( ResistanceAttributesPrefix, "Lightning", "ValueTag: Lightning Resistance" );
	Attributes_Resistance_Physical = AddGameplayTag( ResistanceAttributesPrefix, "Physical", "ValueTag: Physical Resistance" );

	// Damage Values Tags
	Values_Damage_Arcane = AddGameplayTag( DamageTypePrefix, "Arcane", "ValueTag: Arcane Damage" );
	Values_Damage_Fire = AddGameplayTag( DamageTypePrefix, "Fire", "ValueTag: Fire Damage" );
	Values_Damage_Lightning = AddGameplayTag( DamageTypePrefix, "Lightning", "ValueTag: Lightning Damage" );
	Values_Damage_Physical = AddGameplayTag( DamageTypePrefix, "Physical", "ValueTag: Physical Damage" );

	// Values
	Values_XP = AddGameplayTag( ValuesPrefix, "XP", "ValueTag: XP" );

	// Gameplay Cues
	GameplayCue_FireBlast = AddGameplayTag( FString{}, "GameplayCue.FireBlast", "FireBlast gameplay cue" );

	// Offensive Ability Tags
	Abilities_None = AddGameplayTag( AbilitiesPrefix, "None", "No Ability Tag" );
	Abilities_Attack = AddGameplayTag( AbilitiesPrefix, "Attack", "Attack Ability Tag" );
	Abilities_Summon = AddGameplayTag( AbilitiesPrefix, "Summon", "Summon Ability Tag" );
	Abilities_Fire_FireBolt = AddGameplayTag( AbilitiesFirePrefix, "FireBolt", "Firebolt Ability Tag" );
	Abilities_Fire_FireBlast = AddGameplayTag( AbilitiesFirePrefix, "FireBlast", "Fireblast Ability Tag" );
	Abilities_Lightning_Electrocute = AddGameplayTag( AbilitiesLightningPrefix, "Electrocute", "Electrocute Ability Tag" );
	Abilities_Arcane_ArcaneShards = AddGameplayTag( AbilitiesArcanePrefix, "ArcaneShards", "ArcaneShards Ability Tag" );

	// Passive Ability Tags
	Abilities_Passive_HaloOfProtection = AddGameplayTag( AbilitiesPassivePrefix, "HaloOfProtection", "HaloOfProtection Ability Tag" );
	Abilities_Passive_LifeSiphon = AddGameplayTag( AbilitiesPassivePrefix, "LifeSiphon", "LifeSiphon Ability Tag" );
	Abilities_Passive_ManaSiphon = AddGameplayTag( AbilitiesPassivePrefix, "ManaSiphon", "ManaSiphon Ability Tag" );

	// Status Tag
	Abilities_HitReact = AddGameplayTag( AbilitiesPrefix, "HitReact", "HitReact Ability Tag" );

	// Debuffs Tags
	Debuff = AddGameplayTag( FString{}, "Debuff", "Debuff root category" );
	Debuff_Burn = AddGameplayTag( DebuffPrefix, "Burn", "Burn debuff" );
	Debuff_Stun = AddGameplayTag( DebuffPrefix, "Stun", "Stun debuff" );
	Debuff_Arcane = AddGameplayTag( DebuffPrefix, "Arcane", "Arcane debuff" );
	Debuff_Physical = AddGameplayTag( DebuffPrefix, "Physical", "Physical debuff" );

	Knockback_Chance = AddGameplayTag( KnockbackPrefix, "Chance", "Knockback chance" );
	Knockback_Impulse = AddGameplayTag( KnockbackPrefix, "Impulse", "Knockback impulse = direction * magnitude" );

	// Debuffs Info to Pass it using SetByCallerMagnitude
	Debuff_Chance = AddGameplayTag( DebuffPrefix, "Chance", "Chance to apply a debuff" );
	Debuff_Damage = AddGameplayTag( DebuffPrefix, "Damage", "Damage to get on a tick of a debuff" );
	Debuff_Duration = AddGameplayTag( DebuffPrefix, "Duration", "Duration of a debuff" );
	Debuff_Frequency = AddGameplayTag( DebuffPrefix, "Frequency", "How often tick of a debuff is fired" );

	// Ability Statuses
	Abilities_Status_Locked = AddGameplayTag( AbilitiesPrefix + StatusPrefix, "Locked", "Locked status Tag" );
	Abilities_Status_Eligible = AddGameplayTag( AbilitiesPrefix + StatusPrefix, "Eligible", "Eligible status Tag" );
	Abilities_Status_Unlocked = AddGameplayTag( AbilitiesPrefix + StatusPrefix, "Unlocked", "Unlocked status Tag" );
	Abilities_Status_Equipped = AddGameplayTag( AbilitiesPrefix + StatusPrefix, "Equipped", "Equipped status Tag" );

	// Ability Types
	Abilities_Type_Offensive = AddGameplayTag( AbilitiesPrefix + TypePrefix, "Offensive", "Offensive status Tag" );
	Abilities_Type_Passive = AddGameplayTag( AbilitiesPrefix + TypePrefix, "Passive", "Passive status Tag" );
	Abilities_Type_None = AddGameplayTag( AbilitiesPrefix + TypePrefix, "None", "None status Tag" );

	// Cooldowns Tags
	Cooldowns_Fire_FireBolt = AddGameplayTag( CooldownsPrefix, "Fire.FireBolt", "FireBolt Ability CooldownTag" );
	Cooldowns_Lightning_Electrocute = AddGameplayTag( CooldownsPrefix, "Lightning.Electrocute", "Electrocute Ability CooldownTag" );
	Cooldowns_Arcane_ArcaneShards = AddGameplayTag( CooldownsPrefix, "Arcane.ArcaneShards", "ArcaneShards Ability CooldownTag" );

	// Montage Attacks
	Montage_Attack_1 = AddGameplayTag( MontageAttackPrefix, "1", "Montage Attack option 1" );
	Montage_Attack_2 = AddGameplayTag( MontageAttackPrefix, "2", "Montage Attack option 2" );
	Montage_Attack_3 = AddGameplayTag( MontageAttackPrefix, "3", "Montage Attack option 3" );
	Montage_Attack_4 = AddGameplayTag( MontageAttackPrefix, "4", "Montage Attack option 4" );

	// Combat Socket Tags
	CombatSocket_Weapon = AddGameplayTag( CombatSocketPrefix, "Weapon", "Weapon socket attack tag" );
	CombatSocket_LeftHand = AddGameplayTag( CombatSocketPrefix, "LeftHand", "Left hand socket attack tag" );
	CombatSocket_RightHand = AddGameplayTag( CombatSocketPrefix, "RightHand", "Right hand socket attack tag" );
	CombatSocket_Tail = AddGameplayTag( CombatSocketPrefix, "Tail", "Tail socket attack tag" );

	// Meta Attributes Tags
	Attributes_Meta_IncomingXP = AddGameplayTag( MetaAttributesPrefix, "IncomingXP", "IncomingXP meta attribute tag" );

	// Player Block Tags
	Player_Block_InputPressed = AddGameplayTag( PlayerBlockPrefix, "InputPressed", "Tag to block InputPressed" );
	Player_Block_InputReleased = AddGameplayTag( PlayerBlockPrefix, "InputReleased", "Tag to block InputReleased" );
	Player_Block_InputHeld = AddGameplayTag( PlayerBlockPrefix, "InputHeld", "Tag to block InputHeld" );
	Player_Block_CursorTrace = AddGameplayTag( PlayerBlockPrefix, "CursorTrace", "Tag to block CursorTrace" );

	// Associations:
	// Associate Damage with Resistance
	DamageTypeToDamageRes.Add( Values_Damage_Arcane, Attributes_Resistance_Arcane );
	DamageTypeToDamageRes.Add( Values_Damage_Fire, Attributes_Resistance_Fire );
	DamageTypeToDamageRes.Add( Values_Damage_Lightning, Attributes_Resistance_Lightning );
	DamageTypeToDamageRes.Add( Values_Damage_Physical, Attributes_Resistance_Physical );

	// Associate Damage with Resistance
	DamageTypeToDebuff.Add( Values_Damage_Arcane, Debuff_Arcane );
	DamageTypeToDebuff.Add( Values_Damage_Fire, Debuff_Burn );
	DamageTypeToDebuff.Add( Values_Damage_Lightning, Debuff_Stun );
	DamageTypeToDebuff.Add( Values_Damage_Physical, Debuff_Physical );
}
FGameplayTag FAuraGameplayTags::AddGameplayTag( const FString& AttributeNamePrefix, const FString& AttributeName, const FString& AttributeHint )
{
	FName AttributeFullName{ AttributeNamePrefix + AttributeName };
	return UGameplayTagsManager::Get().AddNativeGameplayTag( AttributeFullName, AttributeHint );
}