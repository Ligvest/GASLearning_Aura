// Dovzhik Tolya

#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
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
}
FGameplayTag FAuraGameplayTags::AddGameplayTag( const FString& AttributeNamePrefix, const FString& AttributeName, const FString& AttributeHint )
{
	FName AttributeFullName( AttributeNamePrefix + AttributeName );
	return UGameplayTagsManager::Get().AddNativeGameplayTag( AttributeFullName, AttributeHint );
}