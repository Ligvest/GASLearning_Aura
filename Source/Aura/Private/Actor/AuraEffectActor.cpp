// Dovzhik Tolya

#include "Actor/AuraEffectActor.h"

#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent( CreateDefaultSubobject<USceneComponent>( "RootSceneComponent" ) );
}
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}
void AAuraEffectActor::ApplyEffectToTarget( AActor* TargetActor ) const
{
	const IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>( TargetActor );
	if ( !ASInterface )
	{
		return;
	}
	check( GameplayEffectClass );

	UAbilitySystemComponent* ASC = ASInterface->GetAbilitySystemComponent();

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject( this );
	constexpr float TestLevel = 1.f;
	const FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec( GameplayEffectClass, TestLevel, Context );
	ASC->ApplyGameplayEffectSpecToTarget( *EffectSpec.Data, ASC );
}
