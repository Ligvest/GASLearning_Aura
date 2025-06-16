// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/AuraCharacterBase.h"

#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>( "WeaponMeshComponent" );
	WeaponMeshComponent->SetupAttachment( GetMesh(), SocketNameHandWeapon );
	WeaponMeshComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	GetCapsuleComponent()->SetCollisionResponseToChannel( ECC_Projectile, ECR_Overlap );
	GetCapsuleComponent()->SetCollisionResponseToChannel( ECC_Camera, ECR_Ignore );
	GetCapsuleComponent()->SetCollisionResponseToChannel( ECC_Visibility, ECR_Block );
	GetCapsuleComponent()->SetGenerateOverlapEvents( true );

	GetMesh()->SetCollisionResponseToChannel( ECC_Projectile, ECR_Ignore );
	GetMesh()->SetCollisionResponseToChannel( ECC_Camera, ECR_Ignore );
	GetMesh()->SetGenerateOverlapEvents( false );
}
FVector AAuraCharacterBase::GetProjectileSpawnSocketLocation() const
{
	check( WeaponMeshComponent );
	check( !SocketNameProjectileSpawn.IsNone() );
	// On the server the location of Socket will be default ( not the position in this exact moment of the animation )
	// So to fix this I should pass the position to server it seems. Or ask server to play animations too
	FVector SocketLocation = WeaponMeshComponent->GetSocketLocation( SocketNameProjectileSpawn );
	return SocketLocation;
}
void AAuraCharacterBase::GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
	DOREPLIFETIME( AAuraCharacterBase, CharacterLevel );
}

void AAuraCharacterBase::InitDefaultAttributes( int InCharacterLevel ) const
{
	ApplyEffectToSelf( InitPrimaryAttributesEffectClass );
	ApplyEffectToSelf( InitSecondaryAttributesEffectClass );
	ApplyEffectToSelf( InitVitalAttributesEffectClass );
}
void AAuraCharacterBase::ApplyEffectToSelf( TSubclassOf<UGameplayEffect> EffectClass, float EffectLevel /*= 1.f*/ ) const
{
	// Must be set in child classes
	check( EffectClass );
	// Must be called after ASC is correctly initialized
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check( ASC );
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject( this );
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec( EffectClass, EffectLevel, EffectContextHandle );
	ASC->ApplyGameplayEffectSpecToSelf( *EffectSpecHandle.Data );
}
void AAuraCharacterBase::GrantDefaultAbilities() const
{
	// Run this only on a server
	if ( !HasAuthority() )
	{
		return;
	}

	UAuraAbilitySystemComponent* ASC = CastChecked<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	ASC->GrantAbilities( DefaultAbilityClasses );
}
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
void AAuraCharacterBase::Rep_CharacterLevel( int OldCharacterLevel )
{
}