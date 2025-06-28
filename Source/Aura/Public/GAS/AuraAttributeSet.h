// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

// Accessors
#define ATTRIBUTE_ACCESSORS( ClassName, PropertyName )           \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER( ClassName, PropertyName ) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER( PropertyName )               \
	GAMEPLAYATTRIBUTE_VALUE_SETTER( PropertyName )               \
	GAMEPLAYATTRIBUTE_VALUE_INITTER( PropertyName )

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	// Here are only functions and TObjectPtr in the Handle. So we can just copy it
	FGameplayEffectContextHandle EffectContextHandle;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<AActor> AvatarActor;
	UPROPERTY()
	TObjectPtr<AController> Controller;
	UPROPERTY()
	TObjectPtr<ACharacter> Character;
};

// typedef is specific to the FGameplayAttribute() signature, but TStaticFunPtr is generic to any signature chosen
// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

// TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

/**
 *
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	// If in UPROPERTY we specify HOW the field is replicated in the function we specify WHEN the prop is replicated
	virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

	// Is executed on both Server and Client
	virtual void PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue ) override;
	// Is executed only on Server
	virtual void PostGameplayEffectExecute( const struct FGameplayEffectModCallbackData& Data ) override;

	TMap<FGameplayTag, FGameplayAttribute> TagsToAttributes;

	// Health
	// We can't just use Replicate becuase attributes replication in GAS should be forwarded to the GAS itself in the rep function
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes" )
	FGameplayAttributeData Health;
	// Just to be able to get Health attribute or value, Set Base and current Health
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, Health );

	// +Max Health
	// Based on:
	// Vigor
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes" )
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, MaxHealth );

	// +Health regen per 1 sec
	// Based on:
	// Vigor
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Vital Attributes" )
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, HealthRegeneration );

	// Mana
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes" )
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, Mana );

	// +Max Mana
	// Based on:
	// Intelligence
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes" )
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, MaxMana );

	// +Mana regen per 1 sec
	// Based on:
	// Intelligence
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Vital Attributes" )
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, ManaRegeneration );

	// Primary Attributes
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes" )
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, Strength );

	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes" )
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, Intelligence );

	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes" )
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, Resilience );

	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes" )
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, Vigor );

	// Secondary Attributes
	// -Damage taken, +Block chance
	// Based on:
	// Resilience
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes" )
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, Armor );

	// -Enemy armor, +Crit chance
	// Based on:
	// Resilience
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes" )
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, ArmorPenetration );

	// +Chance to half incoming damage
	// Based on:
	// Armor
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes" )
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, BlockChance );

	// +Chance to crit
	// Based on:
	// Armor Penetration
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes" )
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, CriticalHitChance );

	// +Crit damage
	// Based on:
	// Armor Penetration
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes" )
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, CriticalHitDamage );

	// -Crit chance on taken damage
	// Based on:
	// Armor
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes" )
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, CriticalHitResistance );

	// Percent of ignored Arcane damage
	// Based on:
	// Resilience
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Secondary Attributes" )
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, ArcaneResistance );

	// Percent of ignored Fire damage
	// Based on:
	// Resilience
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Secondary Attributes" )
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, FireResistance );

	// Percent of ignored Lightning damage
	// Based on:
	// Resilience
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Secondary Attributes" )
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, LightningResistance );

	// Percent of ignored Physical damage
	// Based on:
	// Resilience
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Secondary Attributes" )
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, PhysicalResistance );

	UPROPERTY( BlueprintReadOnly, Category = "Meta Attributes" )
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, IncomingDamage );

public:
	// Health
	// When using ReplicatedUsing - after server changes a value it updates Health and calls this function
	// So in OldHealth will be previous value of client's Health before replication
	UFUNCTION()
	void OnRep_Health( const FGameplayAttributeData& OldHealth ) const;
	UFUNCTION()
	void OnRep_MaxHealth( const FGameplayAttributeData& OldMaxHealth ) const;
	UFUNCTION()
	void OnRep_HealthRegeneration( const FGameplayAttributeData& OldHealthRegeneration ) const;

	// Mana
	UFUNCTION()
	void OnRep_Mana( const FGameplayAttributeData& OldMana ) const;
	UFUNCTION()
	void OnRep_MaxMana( const FGameplayAttributeData& OldMaxMana ) const;
	UFUNCTION()
	void OnRep_ManaRegeneration( const FGameplayAttributeData& OldManaRegeneration ) const;

	// Primary attributes
	UFUNCTION()
	void OnRep_Strength( const FGameplayAttributeData& OldStrength ) const;
	UFUNCTION()
	void OnRep_Intelligence( const FGameplayAttributeData& OldIntelligence ) const;
	UFUNCTION()
	void OnRep_Resilience( const FGameplayAttributeData& OldResilience ) const;
	UFUNCTION()
	void OnRep_Vigor( const FGameplayAttributeData& OldVigor ) const;

	// Secondary attributes
	UFUNCTION()
	void OnRep_Armor( const FGameplayAttributeData& OldArmor ) const;
	UFUNCTION()
	void OnRep_ArmorPenetration( const FGameplayAttributeData& OldArmorPenetration ) const;
	UFUNCTION()
	void OnRep_BlockChance( const FGameplayAttributeData& OldBlockChance ) const;
	UFUNCTION()
	void OnRep_CriticalHitChance( const FGameplayAttributeData& OldCriticalHitChance ) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage( const FGameplayAttributeData& OldCriticalHitDamage ) const;
	UFUNCTION()
	void OnRep_CriticalHitResistance( const FGameplayAttributeData& OldCriticalHitResistance ) const;

	// Resistance attributes
	UFUNCTION()
	void OnRep_ArcaneResistance( const FGameplayAttributeData& OldArcaneResistance ) const;
	UFUNCTION()
	void OnRep_FireResistance( const FGameplayAttributeData& OldFireResistance ) const;
	UFUNCTION()
	void OnRep_LightningResistance( const FGameplayAttributeData& OldLightningResistance ) const;
	UFUNCTION()
	void OnRep_PhysicalResistance( const FGameplayAttributeData& OldPhysicalResistance ) const;

protected:
	FEffectProperties EffectTargetProperties;
	FEffectProperties EffectSourceProperties;

	void FillEffectPropertiesWithASC( FEffectProperties& Properties, UAbilitySystemComponent* ASC, const FGameplayEffectContextHandle ContextHandle );
};