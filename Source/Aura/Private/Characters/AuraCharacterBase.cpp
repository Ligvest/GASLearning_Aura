// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/AuraCharacterBase.h"

#include "AuraGameplayTags.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"
#include "GAS/Data/AuraCharacterClassInfoDA.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerState.h"

AAuraCharacterBase::AAuraCharacterBase() : CharacterClass( ECharacterClass::Empty )
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
	GetMesh()->SetCollisionResponseToChannel( ECC_Pawn, ECR_Ignore );
	GetMesh()->SetGenerateOverlapEvents( false );
}

int AAuraCharacterBase::GetCharacterLevel() const
{
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>( GetPlayerState() );
	return AuraPS->GetPlayerLevel();
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation( FGameplayTag MontageAttackTag ) const
{
	// On the server the location of Socket will be default ( not the position in this exact moment of the animation )
	// So to fix this I should pass the position to server it seems. Or ask server to play animations too
	// We choose to play animations on server
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FVector SocketLocation;
	if ( MontageAttackTag == GameplayTags.CombatSocket_Weapon )
	{
		check( WeaponMeshComponent );
		check( !WeaponCombatSocketName.IsNone() );
		SocketLocation = WeaponMeshComponent->GetSocketLocation( WeaponCombatSocketName );
		return SocketLocation;
	}

	if ( MontageAttackTag == GameplayTags.CombatSocket_LeftHand )
	{
		check( !LeftHandCombatSocketName.IsNone() );
		SocketLocation = GetMesh()->GetSocketLocation( LeftHandCombatSocketName );
		return SocketLocation;
	}

	if ( MontageAttackTag == GameplayTags.CombatSocket_RightHand )
	{
		check( !RightHandCombatSocketName.IsNone() );
		SocketLocation = GetMesh()->GetSocketLocation( RightHandCombatSocketName );
		return SocketLocation;
	}

	if ( MontageAttackTag == GameplayTags.CombatSocket_Tail )
	{
		check( !TailCombatSocketName.IsNone() );
		SocketLocation = GetMesh()->GetSocketLocation( TailCombatSocketName );
		return SocketLocation;
	}

	// We shouldn't get here
	check( false );
	return SocketLocation;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	MulticastHandleDeath();
}
bool AAuraCharacterBase::IsDead_Implementation()
{
	return IsDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

FTaggedMontage AAuraCharacterBase::GetRandAttackMontage_Implementation()
{
	constexpr int MinIndex = 0;
	int MaxIndex = AttackMontages.Num() - 1;
	int ResultIndex = FMath::RandRange( MinIndex, MaxIndex );
	return AttackMontages[ResultIndex];
}

UNiagaraSystem* AAuraCharacterBase::GetHurtNSEffect_Implementation()
{
	return HurtNSEffect;
}

USoundBase* AAuraCharacterBase::GetHurtSound_Implementation()
{
	return HurtSound;
}

int AAuraCharacterBase::GetMinionsCount_Implementation()
{
	return MinionsCount;
}

void AAuraCharacterBase::AddMinionsCount_Implementation( int Value )
{
	MinionsCount += Value;
}

void AAuraCharacterBase::SetMasterActor_Implementation( AActor* InMasterActor )
{
	MasterActor = CastChecked<AAuraCharacterBase>( InMasterActor );
}
ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}

FTaggedMontage AAuraCharacterBase::FindAttackMontageByTag_Implementation( FGameplayTag InMontageTag )
{
	for ( const FTaggedMontage& Montage : AttackMontages )
	{
		if ( Montage.MontageTag.MatchesTagExact( InMontageTag ) )
		{
			return Montage;
		}
	}

	// Not sure if I should be able to process the case when there is no Montage with the tag
	check( false );
	return FTaggedMontage{};
}

void AAuraCharacterBase::DissolveCorpse()
{
	TArray<UMaterialInstanceDynamic*> DynamicMatInstances;
	if ( IsValid( DissolveMaterialInstance ) )
	{
		UMaterialInstanceDynamic* MeshDissolveMaterial = UMaterialInstanceDynamic::Create( DissolveMaterialInstance, this );
		constexpr int MaterialIndex = 0;
		GetMesh()->SetMaterial( MaterialIndex, MeshDissolveMaterial );
		DynamicMatInstances.Add( MeshDissolveMaterial );
	}

	if ( IsValid( WeaponDissolveMaterialInstance ) )
	{
		UMaterialInstanceDynamic* WeaponDissolveMaterial = UMaterialInstanceDynamic::Create( WeaponDissolveMaterialInstance, this );
		constexpr int MaterialIndex = 0;
		WeaponMeshComponent->SetMaterial( MaterialIndex, WeaponDissolveMaterial );
		DynamicMatInstances.Add( WeaponDissolveMaterial );
	}

	if ( !DynamicMatInstances.IsEmpty() )
	{
		StartDissolving( DynamicMatInstances );
	}
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	check( DeathSound );
	UGameplayStatics::SpawnSoundAtLocation( this, DeathSound, GetActorLocation(), GetActorRotation() );

	WeaponMeshComponent->DetachFromComponent( FDetachmentTransformRules( EDetachmentRule::KeepWorld, true ) );
	WeaponMeshComponent->SetSimulatePhysics( true );
	WeaponMeshComponent->SetEnableGravity( true );
	WeaponMeshComponent->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	CharacterMesh->SetSimulatePhysics( true );
	CharacterMesh->SetEnableGravity( true );
	CharacterMesh->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );

	GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	DissolveCorpse();
	IsDead = true;

	if ( IsValid( MasterActor ) )
	{
		// As this character dies it should decrement minions counter if it has a MasterActor which spawned it
		--MasterActor->MinionsCount;
	}
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
	constexpr int DefaultAbilityLevel = 1;
	ASC->GrantAbilities( DefaultAbilityClasses, DefaultAbilityLevel );

	AAuraGameModeBase* AuraGM = CastChecked<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) );
	UAuraCharacterClassInfoDA* DefaultCharacterInfoDA = AuraGM->GetDefaultCharacterInfoDA();
	const TArray<TSubclassOf<UGameplayAbility>>& CharacterClassStartupAbilities = DefaultCharacterInfoDA->GetClassDefaultInfo( CharacterClass ).StartupAbilityClasses;
	ASC->GrantAbilities( CharacterClassStartupAbilities, GetCharacterLevel() );

	ASC->GrantPassiveAbilities( DefaultPassiveAbilityClasses, DefaultAbilityLevel );
}
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
void AAuraCharacterBase::Rep_CharacterLevel( int OldCharacterLevel )
{
}