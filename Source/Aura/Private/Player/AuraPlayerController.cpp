// Dovzhik Tolya

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	// To allow our client version of player controller to control the server version of our player controller
	bReplicates = true;
}
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check( IMC );

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( GetLocalPlayer() );
	check( InputSubsystem );

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
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>( InputComponent );
	check( MoveAction );
	EnhancedInputComponent->BindAction( MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move );
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