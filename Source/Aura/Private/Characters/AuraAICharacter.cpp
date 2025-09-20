// Dovzhik Tolya

#include "Characters/AuraAICharacter.h"

#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interaction/HighlightActorHelper.h"

void AAuraAICharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth( true );
	GetMesh()->MarkRenderStateDirty();
	if ( IsValid( WeaponMeshComponent ) )
	{
		WeaponMeshComponent->SetRenderCustomDepth( true );
		WeaponMeshComponent->MarkRenderStateDirty();
	}
}
void AAuraAICharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth( false );
	GetMesh()->MarkRenderStateDirty();
	if ( IsValid( WeaponMeshComponent ) )
	{
		WeaponMeshComponent->SetRenderCustomDepth( false );
		WeaponMeshComponent->MarkRenderStateDirty();
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
void AAuraAICharacter::PossessedBy( AController* NewController )
{
	Super::PossessedBy( NewController );
}