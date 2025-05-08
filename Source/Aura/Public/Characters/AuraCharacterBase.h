// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UGameplayEffect;
// Forward declarations
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	//~ Begin of IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	//~ End of IAbilitySystemInterface

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY( EditDefaultsOnly, Category = SocketNames )
	FName SocketNameHandWeapon = FName( "HandWeapon" );

	UPROPERTY( EditAnywhere, Category = Combat )
	TObjectPtr<class USkeletalMeshComponent> WeaponMeshComponent;

	void InitDefaultAttributes() const;

	void ApplyEffectToSelf( TSubclassOf<UGameplayEffect> EffectClass, float EffectLevel = 1.f ) const;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesEffectClass;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UGameplayEffect> InitSecondaryAttributesEffectClass;

	virtual void BeginPlay() override;
};