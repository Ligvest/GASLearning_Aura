// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/AuraCharacterBase.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMeshComponent = CreateDefaultSubobject<class USkeletalMeshComponent>( "WeaponMeshComponent" );
	WeaponMeshComponent->SetupAttachment( GetMesh(), SocketNameHandWeapon );
	WeaponMeshComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

void AAuraCharacterBase::InitDefaultAttributes() const
{
	ApplyEffectToSelf( InitPrimaryAttributesEffectClass );
	ApplyEffectToSelf( InitSecondaryAttributesEffectClass );
}
void AAuraCharacterBase::ApplyEffectToSelf( TSubclassOf<UGameplayEffect> EffectClass, float EffectLevel /*= 1.f*/ ) const
{
	// Must be set in child classes
	check( EffectClass );
	// Must be called after ASC is correctly initialized
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check( ASC );
	const FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec( EffectClass, EffectLevel, EffectContextHandle );
	ASC->ApplyGameplayEffectSpecToTarget( *EffectSpecHandle.Data, ASC );
}
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}