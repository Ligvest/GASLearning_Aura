// Dovzhik Tolya

#include "Characters/AuraPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

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

// Server
void AAuraPlayerCharacter::PossessedBy( AController* NewController )
{
	Super::PossessedBy( NewController );

	// For initializing AbilityComponent we use PossessedBy because without controlling the character the abilities don't make sense
	// Init ability actor info for Server
	InitGASInfoAndHUD();
	GrantDefaultAbilities();
}

// Client
void AAuraPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For initializing AbilityComponent we use OnRep_PlayerState instead of AknowledgePossession on a client
	// because we need to be sure that PlayerState is replicated and has all valid data so we can use it
	// Init ability actor info for Client
	InitGASInfoAndHUD();
}
void AAuraPlayerCharacter::InitGASInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// TODO: Debug begin
	APlayerController* D1PlayerController = GetLocalViewingPlayerController();
	APlayerController* D2PlayerController = GetWorld()->GetFirstPlayerController();
	APlayerController* D3PlayerController = UGameplayStatics::GetPlayerController( GetWorld(), 0 );
	APlayerController* D4PlayerController = UGameplayStatics::GetPlayerController( this, 0 );
	APlayerController* D5PlayerController = UGameplayStatics::GetPlayerController( AuraPlayerState, 0 );
	// TODO: Debug end
	check( AuraPlayerState );
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo( AuraPlayerState, this );
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	check( AuraASC );
	AuraASC->Init();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	// It can be called only on the server as attributes will be replicated regardless
	InitDefaultAttributes();
}
void AAuraPlayerCharacter::InitHUD() const
{
	const APlayerController* PC = Cast<APlayerController>( GetController() );
	// TODO: Debug
	APlayerState* D1PC = GetPlayerState();
	APlayerController* D1PlayerController = GetLocalViewingPlayerController();
	APlayerController* D2PlayerController = GetWorld()->GetFirstPlayerController();
	APlayerController* D3PlayerController = UGameplayStatics::GetPlayerController( GetWorld(), 0 );
	APlayerController* D4PlayerController = UGameplayStatics::GetPlayerController( this, 0 );
	APlayerController* D5PlayerController = UGameplayStatics::GetPlayerController( D1PC, 0 );
	// TODO: Debug end
	AAuraHUD* HUD = PC->GetHUD<AAuraHUD>();
	check( HUD );
	HUD->InitHUDWidget();
}
void AAuraPlayerCharacter::InitGASInfoAndHUD()
{
	InitGASInfo();

	// Init HUD only if this is a client. And this client controls this character ( so that it has valid PlayerController )
	if ( IsLocallyControlled() )
	{
		InitHUD();
	}
}