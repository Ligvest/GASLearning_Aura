// Dovzhik Tolya

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Containers/Map.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent( CreateDefaultSubobject<USceneComponent>( "RootSceneComponent" ) );
}
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}
void AAuraEffectActor::ApplyEffectToTarget( AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass )
{
	const IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>( TargetActor );
	if ( !ASInterface )
	{
		return;
	}
	check( GameplayEffectClass );

	UAbilitySystemComponent* ASC = ASInterface->GetAbilitySystemComponent();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject( this );
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec( GameplayEffectClass, EffectLevel, ContextHandle );
	const FActiveGameplayEffectHandle GEHandle = ASC->ApplyGameplayEffectSpecToSelf( *EffectSpecHandle.Data );

	const bool bIsInfinite = EffectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if ( bIsInfinite )
	{
		AscToInfiniteGEHandle.Add( ASC, GEHandle );
	}
}

void AAuraEffectActor::RemoveInfiniteEffectFromTarget( AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass )
{
	const IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>( TargetActor );
	if ( !ASInterface )
	{
		return;
	}

	check( GameplayEffectClass );

	UAbilitySystemComponent* ASC = ASInterface->GetAbilitySystemComponent();
	FActiveGameplayEffectHandle GEHandle;
	const bool bFoundAndRemoved = AscToInfiniteGEHandle.RemoveAndCopyValue( ASC, GEHandle );
	if ( bFoundAndRemoved )
	{
		ASC->RemoveActiveGameplayEffect( GEHandle );
	}
}

void AAuraEffectActor::OnBeginOverlap( AActor* TargetActor )
{
	// Instant gameplay effect
	if ( InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap )
	{
		ApplyEffectToTarget( TargetActor, InstantGameplayEffectClass );
	}

	// Duration gameplay effect
	if ( DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap )
	{
		ApplyEffectToTarget( TargetActor, DurationGameplayEffectClass );
	}

	// Infinite gameplay effect
	if ( InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap )
	{
		ApplyEffectToTarget( TargetActor, InfiniteGameplayEffectClass );
	}

	if ( bPickupable )
	{
		// Todo: maybe not a good way to do this
		Destroy();
	}
}
void AAuraEffectActor::OnEndOverlap( AActor* TargetActor )
{
	// Instant gameplay effect
	if ( InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap )
	{
		ApplyEffectToTarget( TargetActor, InstantGameplayEffectClass );
	}

	// Duration gameplay effect
	if ( DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap )
	{
		ApplyEffectToTarget( TargetActor, DurationGameplayEffectClass );
	}

	// Infinite gameplay effect
	if ( InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap )
	{
		ApplyEffectToTarget( TargetActor, InfiniteGameplayEffectClass );
	}

	if ( InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap )
	{
		RemoveInfiniteEffectFromTarget( TargetActor, InfiniteGameplayEffectClass );
	}
}
