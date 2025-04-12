// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

// Forward declarations
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY( EditDefaultsOnly, Category = SocketNames )
	FName SocketNameHandWeapon = FName( "HandWeapon" );

	UPROPERTY( EditAnywhere, Category = Combat )
	TObjectPtr<class USkeletalMeshComponent> WeaponMeshComponent;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void BeginPlay() override;
};