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

	// We can't just use Replicate becuase attributes replication in GAS should be forwarded to the GAS itself in the rep function
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes" )
	FGameplayAttributeData Health;
	// Just to be able to get Health attribute or value, Set Base and current Health
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, Health );
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes" )
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, MaxHealth );
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes" )
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, Mana );
	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes" )
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS( UAuraAttributeSet, MaxMana );

	// When using ReplicatedUsing - after server changes a value it updates Health and calls this function
	// So in OldHealth will be previous value of client's Health before replication
	UFUNCTION()
	void OnRep_Health( const FGameplayAttributeData& OldHealth ) const;
	UFUNCTION()
	void OnRep_MaxHealth( const FGameplayAttributeData& OldMaxHealth ) const;
	UFUNCTION()
	void OnRep_Mana( const FGameplayAttributeData& OldMana ) const;
	UFUNCTION()
	void OnRep_MaxMana( const FGameplayAttributeData& OldMaxMana ) const;
};