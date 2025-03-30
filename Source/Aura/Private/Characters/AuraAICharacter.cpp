// Dovzhik Tolya

#include "Characters/AuraAICharacter.h"
#include "Interaction/HighlightActorHelper.h"

void AAuraAICharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth( true );
	if ( IsValid( WeaponMeshComponent ) )
	{
		WeaponMeshComponent->SetRenderCustomDepth( true );
	}
}
void AAuraAICharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth( false );
	if ( IsValid( WeaponMeshComponent ) )
	{
		WeaponMeshComponent->SetRenderCustomDepth( false );
	}
}
void AAuraAICharacter::BeginPlay()
{
	Super::BeginPlay();

	int DepthStencilValue = FHighlightActorHelper::HighlightActorTypeToStencilValue( HighlightActorType );
	GetMesh()->SetCustomDepthStencilValue( DepthStencilValue );
	if ( IsValid( WeaponMeshComponent ) )
	{
		WeaponMeshComponent->SetCustomDepthStencilValue( DepthStencilValue );
	}
}