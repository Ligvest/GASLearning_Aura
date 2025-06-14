// Dovzhik Tolya

#include "Characters/AuraEnemyCharacter.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

AAuraEnemyCharacter::AAuraEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>( "AuraAbilitySystemComponent" );
	// #lig Replication
	AbilitySystemComponent->SetIsReplicated( true );
	AbilitySystemComponent->SetReplicationMode( EGameplayEffectReplicationMode::Minimal );

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>( "AuraAttributeSet" );
}
void AAuraEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo( this, this );
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	check( AuraASC );
	AuraASC->Init();
	InitDefaultAttributes();
}