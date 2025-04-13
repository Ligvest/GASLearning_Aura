// Dovzhik Tolya

#include "Actor/AuraEffectActor.h"

#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( "StaticMeshComponent" );
	// Making StaticMeshComponent the root in the actors hierarchy
	SetRootComponent( StaticMeshComponent );

	SphereComponent = CreateDefaultSubobject<USphereComponent>( "SphereComponent" );
	SphereComponent->SetupAttachment( GetRootComponent() );
}

void AAuraEffectActor::OnBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult )
{
	// TODO: This is just a test workflow and I MUST change it to work with GameplayEffects instead of changing values directly!!!
	// If overlapped actor has AbilitySystemComponent
	if ( IAbilitySystemInterface* ActorWithASC = Cast<IAbilitySystemInterface>( OtherActor ) )
	{
		UAbilitySystemComponent* ASC = ActorWithASC->GetAbilitySystemComponent();
		// The component should be already valid after possesion
		check( ASC );
		const UAttributeSet* AttributeSet = ASC->GetAttributeSet( UAuraAttributeSet::StaticClass() );
		const UAuraAttributeSet* AuraAttributeSetConst = Cast<UAuraAttributeSet>( AttributeSet );
		UAuraAttributeSet* AuraAttributeSet = const_cast<UAuraAttributeSet*>( AuraAttributeSetConst );
		constexpr float HealthToAdd = 25.f;
		AuraAttributeSet->SetHealth( AuraAttributeSet->GetHealth() + HealthToAdd );
		// Destroy the actor after picking it up
		Destroy();
	}
}
void AAuraEffectActor::OnEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
}
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic( this, &AAuraEffectActor::OnBeginOverlap );
	SphereComponent->OnComponentEndOverlap.AddDynamic( this, &AAuraEffectActor::OnEndOverlap );
}
