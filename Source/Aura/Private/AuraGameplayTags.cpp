// Dovzhik Tolya

#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// Input
	AuraInput_LBM = AddGameplayTag( InputPrefix, "LBM", "Left Mouse Button" );
	AuraInput_RBM = AddGameplayTag( InputPrefix, "RBM", "Right Mouse Button" );
	AuraInput_1 = AddGameplayTag( InputPrefix, "1", "Key \"1\"" );
	AuraInput_2 = AddGameplayTag( InputPrefix, "2", "Key \"2\"" );
	AuraInput_3 = AddGameplayTag( InputPrefix, "3", "Key \"3\"" );
	AuraInput_4 = AddGameplayTag( InputPrefix, "4", "Key \"4\"" );

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

	// Associate Damage with Resistance
	DamageTypeToDamageRes.Add( Values_Damage_Arcane, Attributes_Resistance_Arcane );
	DamageTypeToDamageRes.Add( Values_Damage_Fire, Attributes_Resistance_Fire );
	DamageTypeToDamageRes.Add( Values_Damage_Lightning, Attributes_Resistance_Lightning );
	DamageTypeToDamageRes.Add( Values_Damage_Physical, Attributes_Resistance_Physical );
}
FGameplayTag FAuraGameplayTags::AddGameplayTag( const FString& AttributeNamePrefix, const FString& AttributeName, const FString& AttributeHint )
{
	FName AttributeFullName( AttributeNamePrefix + AttributeName );
	return UGameplayTagsManager::Get().AddNativeGameplayTag( AttributeFullName, AttributeHint );
}