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
class UDebuffNiagaraComponent;

UCLASS() class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual void BeginPlay() override;

	//~ Begin of IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	//~ End of IAbilitySystemInterface

	//~ Begin of ICombatInterface
	virtual int GetCharacterLevel() const override;
	virtual FVector GetCombatSocketLocation_Implementation( FGameplayTag MontageAttackTag ) const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die( FVector DeathImpulse ) override;
	virtual bool IsDead_Implementation() override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual FTaggedMontage GetRandAttackMontage_Implementation() override;
	virtual FTaggedMontage FindAttackMontageByTag_Implementation( FGameplayTag InMontageTag ) override;
	virtual UNiagaraSystem* GetHurtNSEffect_Implementation() override;
	virtual USoundBase* GetHurtSound_Implementation() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual int GetMinionsCount_Implementation() override;
	virtual void AddMinionsCount_Implementation( int Value ) override;
	virtual void SetMasterActor_Implementation( AActor* InMasterActor ) override;
	virtual ECharacterClass GetCharacterClass_Implementation() const override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual void SetIsBeingShocked_Implementation( bool bInShock ) override;
	virtual bool IsBeingShocked_Implementation() const override;
	//~ End of ICombatInterface

	void DissolveCorpse();

	UFUNCTION( BlueprintImplementableEvent, Category = "Death" )
	void StartDissolving( const TArray<UMaterialInstanceDynamic*>& DynamicMatInstances );

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void InitDefaultAttributes( int InCharacterLevel ) const;

	UFUNCTION( BlueprintCallable, Category = "GAS" )
	void ApplyEffectToSelf( TSubclassOf<UGameplayEffect> EffectClass, float EffectLevel = 1.f ) const;

	void GrantDefaultAbilities() const;

	virtual void StunTagChanged( const FGameplayTag CallbackTag, int32 NewCount );

	// Replication
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const;

	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	// Death and dissolve
	UFUNCTION( NetMulticast, Reliable )
	virtual void MulticastHandleDeath( FVector DeathImpulse );

protected:
	// Delegates
	FOnASCRegistered OnAscRegisteredDelegate;
	FOnDeathSignature OnDeathDelegate;

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

	UPROPERTY( EditDefaultsOnly, Category = "Combat" )
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY( EditDefaultsOnly, Category = "Combat" )
	TObjectPtr<USoundBase> HurtSound;

	UPROPERTY( EditDefaultsOnly, Category = "Combat" )
	TObjectPtr<UNiagaraSystem> HurtNSEffect;

	UPROPERTY( EditDefaultsOnly, Category = "Combat" )
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffNSComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Combat" )
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffNSComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Combat" )
	ECharacterClass CharacterClass;

	// Can't be set in Editor as it's used in ctor
	// UPROPERTY( EditDefaultsOnly, Category = "Combat | Socket Names" )
	FName SocketNameHandWeapon = FName( TEXT( "HandWeapon" ) );

	UPROPERTY( EditDefaultsOnly, Category = "Combat | Socket Names" )
	FName WeaponCombatSocketName;

	UPROPERTY( EditDefaultsOnly, Category = "Combat | Socket Names" )
	FName LeftHandCombatSocketName;

	UPROPERTY( EditDefaultsOnly, Category = "Combat | Socket Names" )
	FName RightHandCombatSocketName;

	UPROPERTY( EditDefaultsOnly, Category = "Combat | Socket Names" )
	FName TailCombatSocketName;

	UPROPERTY( EditAnywhere, Category = "Combat" )
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Combat" )
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Combat" )
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Combat" )
	int MinionsCount = 0;

	// If this is a minion then it should have it's MasterActor which summoned it
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Combat" )
	AAuraCharacterBase* MasterActor;

	UPROPERTY( EditDefaultsOnly )
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilityClasses;

	UPROPERTY( EditDefaultsOnly )
	TArray<TSubclassOf<UGameplayAbility>> DefaultPassiveAbilityClasses;

	// Death and Dissolve
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Death" )
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Death" )
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	bool IsDead = false;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	float BaseMaxWalkSpeed = 600.f;

	UPROPERTY( ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly )
	bool bIsStunned = false;

	UPROPERTY( ReplicatedUsing = OnRep_Burned, BlueprintReadOnly )
	bool bIsBurned = false;

	UPROPERTY( Replicated, BlueprintReadOnly )
	bool bIsBeingShocked = false;
};