// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
// Forward declarations
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	//~ Begin of IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	//~ End of IAbilitySystemInterface

	//~ Begin of ICombatInterface
	virtual int GetActorLevel() const override { return GetCharacterLevel(); };
	virtual FVector GetProjectileSpawnSocketLocation() const override;
	//~ End of ICombatInterface

	virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY( EditDefaultsOnly, Category = SocketNames )
	FName SocketNameHandWeapon = FName( "HandWeapon" );

	UPROPERTY( EditDefaultsOnly, Category = SocketNames )
	FName SocketNameProjectileSpawn;

	UPROPERTY( EditAnywhere, Category = Combat )
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	void InitDefaultAttributes() const;

	void ApplyEffectToSelf( TSubclassOf<UGameplayEffect> EffectClass, float EffectLevel = 1.f ) const;

	void GrantDefaultAbilities() const;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesEffectClass;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UGameplayEffect> InitSecondaryAttributesEffectClass;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UGameplayEffect> InitVitalAttributesEffectClass;

	virtual void BeginPlay() override;

	FORCEINLINE int GetCharacterLevel() const { return CharacterLevel; }
	FORCEINLINE void SetCharacterLevel( int NewValue ) { CharacterLevel = NewValue; }

	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = Rep_CharacterLevel )
	int CharacterLevel = 1;

	UFUNCTION()
	void Rep_CharacterLevel( int OldCharacterLevel );

	UPROPERTY( EditDefaultsOnly )
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilityClasses;
};