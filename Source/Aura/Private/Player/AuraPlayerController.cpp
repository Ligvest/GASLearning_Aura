// Dovzhik Tolya

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/AuraPlayerCharacter.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/HighlightActorInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	// To allow our client version of player controller to control the server version of our player controller
	bReplicates = true;
}
void AAuraPlayerController::BeginPlay()
{
	// Server and Client part
	Super::BeginPlay();
	check( IMC );

	// Client only part
	if ( IsLocalPlayerController() )
	{
		BeginPlayClientOnly();
	}
}

inline void AAuraPlayerController::BeginPlayClientOnly()
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( GetLocalPlayer() );

	// Add Input Mapping Context to the player input subsystem so this PlayerController could use this IMC
	InputSubsystem->AddMappingContext( IMC, 0 );

	// Display mouse cursor
	bShowMouseCursor = true;
	// Mouse cursor appearance
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	// Cursor will not be locked inside the game window
	InputModeData.SetLockMouseToViewportBehavior( EMouseLockMode::DoNotLock );
	// Cursor will not be hidden after focusing on game window
	InputModeData.SetHideCursorDuringCapture( false );
	SetInputMode( InputModeData );
}
UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if ( !IsValid( ASC ) )
	{
		AAuraPlayerCharacter* AuraCharacter = CastChecked<AAuraPlayerCharacter>( GetPawn() );
		ASC = Cast<UAuraAbilitySystemComponent>( AuraCharacter->GetAbilitySystemComponent() );
	}

	return ASC;
}
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>( InputComponent );
	check( MoveAction );
	AuraInputComponent->BindAction( MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move );
	AuraInputComponent->BindAbilityActions( InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld );
}
void AAuraPlayerController::AbilityInputTagPressed( FGameplayTag InputTag )
{
	return;
	// if ( IsValid( GetASC() ) )
	// {
	// 	GetASC()->AbilityInputTagPressed( InputTag );
	// }
}
void AAuraPlayerController::AbilityInputTagReleased( FGameplayTag InputTag )
{
	if ( IsValid( GetASC() ) )
	{
		GetASC()->AbilityInputTagReleased( InputTag );
	}
}
void AAuraPlayerController::AbilityInputTagHeld( FGameplayTag InputTag )
{
	// GEngine->AddOnScreenDebugMessage( -1, 2.f, FColor::Yellow, "Holding ability with tag: " + InputTag.ToString() );
	if ( IsValid( GetASC() ) )
	{
		GetASC()->AbilityInputTagHeld( InputTag );
	}
}
void AAuraPlayerController::Move( const FInputActionValue& InputActionValue )
{
	FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	// Get controller rotation.
	// In FPS it could be the camera angle
	// In topdown games rotation can be constant so forward and right vector will be always the same
	const FRotator Rotation = GetControlRotation();
	// Separate Yaw rotation of it
	const FRotator YawRotation( 0.f, Rotation.Yaw, 0.f );

	// Get forward axis in world coordinates. In Unreal Engine forward axis is X when in screen coordinates for example this would be axis Z
	// FRotationMatrix applies YawRotation rotation on identity rotation matrix. And then we get X axis from the result matrix
	const FVector ForwardDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );

	// The same procedure but we get the right axis
	const FVector RightDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

	if ( APawn* ControlledPawn = GetPawn<APawn>() )
	{
		ControlledPawn->AddMovementInput( ForwardDirection, InputAxisVector.Y );
		ControlledPawn->AddMovementInput( RightDirection, InputAxisVector.X );
	}
}
void AAuraPlayerController::CursorTrace()
{
	FHitResult Hit;
	GetHitResultUnderCursor( ECC_Visibility, false, Hit );
	if ( !Hit.bBlockingHit )
	{
		return;
	}

	LastActorUnderCursorToHighlight = ThisActorUnderCursorToHighlight;
	ThisActorUnderCursorToHighlight = Hit.GetActor();

	/*
	 * All cases:
	 * A. ThisActor == nullptr, LastActor == nullptr - do nothing
	 * B. ThisActor == nullptr, LastActor is Valid - UnHighlight Last
	 * C. ThisActor is Valid, LastActor == nullptr - Highlight This
	 * D. ThisActor is Valid, LastActor is Valid, ThisActor == LastActor - Do nothing ( already highlighted )
	 * E. ThisActor is Valid, LastActor is Valid, ThisActor != LastActor - Hightlight This, UnHightlight Last
	 *
	 */

	// If the new actor is the same as the last one, do nothing
	if ( ThisActorUnderCursorToHighlight == LastActorUnderCursorToHighlight )
	{
		return;
	}

	// If there was a previously highlighted actor, unhighlight it
	if ( LastActorUnderCursorToHighlight )
	{
		LastActorUnderCursorToHighlight->UnHighlightActor();
	}

	// If there is a new actor to highlight, highlight it
	if ( ThisActorUnderCursorToHighlight )
	{
		ThisActorUnderCursorToHighlight->HighlightActor();
	}
}
void AAuraPlayerController::PlayerTick( float DeltaSeconds )
{
	Super::PlayerTick( DeltaSeconds );
	CursorTrace();
}
