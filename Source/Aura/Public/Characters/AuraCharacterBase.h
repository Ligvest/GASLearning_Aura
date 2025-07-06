// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

enum class ECharacterClass : uint8;
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

	virtual void BeginPlay() override;

	FORCEINLINE int GetCharacterLevel() const { return CharacterLevel; }
	FORCEINLINE void SetCharacterLevel( int NewValue ) { CharacterLevel = NewValue; }

	UFUNCTION()
	void Rep_CharacterLevel( int OldCharacterLevel );

	//~ Begin of IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	//~ End of IAbilitySystemInterface

	//~ Begin of ICombatInterface
	virtual int GetActorLevel() const override { return GetCharacterLevel(); };
	virtual FVector GetCombatSocketLocation_Implementation( FGameplayTag MontageAttackTag ) const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual bool IsDead_Implementation() override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual FTaggedMontage GetRandAttackMontage_Implementation() override;
	//~ End of ICombatInterface

	virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

	// Death and dissolve

	UFUNCTION( NetMulticast, Reliable )
	virtual void MulticastHandleDeath();

	void DissolveCorpse();

	UFUNCTION( BlueprintImplementableEvent, Category = "Death" )
	void StartDissolving( const TArray<UMaterialInstanceDynamic*>& DynamicMatInstances );

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void InitDefaultAttributes( int InCharacterLevel ) const;

	UFUNCTION( BlueprintCallable, Category = "GAS" )
	void ApplyEffectToSelf( TSubclassOf<UGameplayEffect> EffectClass, float EffectLevel = 1.f ) const;

	void GrantDefaultAbilities() const;

protected:
	UPROPERTY( EditDefaultsOnly, Category = "CharacterDefaults" )
	ECharacterClass CharacterClass;

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

	UPROPERTY( EditDefaultsOnly, Category = SocketNames )
	FName SocketNameHandWeapon = FName( "HandWeapon" );

	UPROPERTY( EditDefaultsOnly, Category = Combat )
	FName WeaponCombatSocketName;

	UPROPERTY( EditDefaultsOnly, Category = Combat )
	FName LeftHandCombatSocketName;

	UPROPERTY( EditDefaultsOnly, Category = Combat )
	FName RightHandCombatSocketName;

	UPROPERTY( EditAnywhere, Category = Combat )
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Combat" )
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Combat" )
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, ReplicatedUsing = Rep_CharacterLevel )
	int CharacterLevel = 1;

	UPROPERTY( EditDefaultsOnly )
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilityClasses;

	// Death and Dissolve
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Death" )
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Death" )
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	bool IsDead = false;
};