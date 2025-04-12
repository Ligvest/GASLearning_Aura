// Dovzhik Tolya

#include "Characters/AuraPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"

AAuraPlayerCharacter::AAuraPlayerCharacter()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	check( MovementComponent );

	// Turn character in the direction of the movement
	MovementComponent->bOrientRotationToMovement = true;
	// Character turn speed
	MovementComponent->RotationRate = FRotator( 0.0f, 400.0f, 0.0f );

	// Allow movement only by X and Y axis ( restrict Z axis )
	// This is default axis. If you want to make 2D game then you can restrict X axis instead
	MovementComponent->SetPlaneConstraintNormal( FVector( 0, 0, 1 ) );
	MovementComponent->bConstrainToPlane = true;
	// Snap to XY plane on start ( so if your character will be spawned higher it won't stuck there)
	MovementComponent->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
void AAuraPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}
void AAuraPlayerCharacter::PossessedBy( AController* NewController )
{
	Super::PossessedBy( NewController );
	// Init ability actor info for Server
	InitAbilityActorInfo();
}
void AAuraPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Init ability actor info for Client
	InitAbilityActorInfo();
}
void AAuraPlayerCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo( AuraPlayerState, this );
	AttributeSet = AuraPlayerState->GetAttributeSet();
}