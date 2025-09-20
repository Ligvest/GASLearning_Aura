// Dovzhik Tolya

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Containers/Map.h"
#include "GAS/AuraGasBpLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent( CreateDefaultSubobject<USceneComponent>( "RootSceneComponent" ) );
}

void AAuraEffectActor::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );
	RunningTime += DeltaSeconds;
	const float SinePeriod = 2 * PI / SinePeriodConstant;
	if ( RunningTime > SinePeriod )
	{
		RunningTime = 0.f;
	}
	ItemMovement( DeltaSeconds );
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	CalculatedRotation = GetActorRotation();
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
	// Don't apply to enemies if the flag is false
	if ( !bApplicableToEnemies && UAuraGasBpLibrary::HasEnemyActorTag( TargetActor ) )
	{
		return;
	}

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
	// Don't apply to enemies if the flag is false
	if ( !bApplicableToEnemies && UAuraGasBpLibrary::HasEnemyActorTag( TargetActor ) )
	{
		return;
	}

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

void AAuraEffectActor::ItemMovement( float DeltaTime )
{
	if ( bRotates )
	{
		const FRotator DeltaRotation( 0.f, DeltaTime * RotationRate, 0.f );
		CalculatedRotation = UKismetMathLibrary::ComposeRotators( CalculatedRotation, DeltaRotation );
	}
	if ( bSinusoidalMovement )
	{
		const float Sine = SineAmplitude * FMath::Sin( RunningTime * SinePeriodConstant );
		CalculatedLocation = InitialLocation + FVector( 0.f, 0.f, Sine );
	}
}

void AAuraEffectActor::StartSinusoidalMovement()
{
	bSinusoidalMovement = true;
	// InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
}

void AAuraEffectActor::StartRotation()
{
	bRotates = true;
	// CalculatedRotation = GetActorRotation();
}
