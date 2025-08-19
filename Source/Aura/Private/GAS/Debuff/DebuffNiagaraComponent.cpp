// Dovzhik Tolya

#include "GAS/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	SetAutoActivate( false );
}
void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* ComponentOwner = GetOwner();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>( ComponentOwner );
	// For now we want to apply debuffs only on actors who has ASC and CombatInterfaces
	check( ComponentOwner->Implements<UCombatInterface>() );

	// Do nothing if the character is dead
	if ( ICombatInterface::Execute_IsDead( ComponentOwner ) )
	{
		return;
	}
	bool bIsServer = ComponentOwner->HasAuthority();

	// Here we check ASC just in case if for some reason it didn't have time to be initialized before getting debuff
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( ComponentOwner );
	if ( ASC )
	{
		ASC->RegisterGameplayTagEvent( DebuffTag, EGameplayTagEventType::NewOrRemoved ).AddUObject( this, &UDebuffNiagaraComponent::OnDebuffTagAddedOrRemoved );
	}
	else
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(
		    this, [this]( UAbilitySystemComponent* InASC )
		    { InASC->RegisterGameplayTagEvent( DebuffTag, EGameplayTagEventType::NewOrRemoved ).AddUObject( this, &UDebuffNiagaraComponent::OnDebuffTagAddedOrRemoved ); } );
	}
}

void UDebuffNiagaraComponent::OnDebuffTagAddedOrRemoved( const FGameplayTag ChangedTag, int32 NewCount )
{
	if ( NewCount > 0 )
	{
		Activate( true );
	}
	else
	{
		Deactivate();
	}
}