// Dovzhik Tolya

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

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
