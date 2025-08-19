// Dovzhik Tolya

#include "Player/AuraPlayerController.h"

#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/AuraPlayerCharacter.h"
#include "Components/SplineComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/HighlightActorInterface.h"
#include "UI/WidgetComponent/DamageTextComponent.h"

void AAuraPlayerController::DrawSplineDebug( USplineComponent* Spline, FColor Color, float Step )
{
	if ( !Spline ) return;

	const float SplineLength = Spline->GetSplineLength();
	for ( float Distance = 0.0f; Distance < SplineLength; Distance += Step )
	{
		const FVector Start = Spline->GetLocationAtDistanceAlongSpline( Distance, ESplineCoordinateSpace::World );
		const FVector End = Spline->GetLocationAtDistanceAlongSpline( Distance + Step, ESplineCoordinateSpace::World );

		DrawDebugLine( GetWorld(), Start, End, Color, false, 0.2f, 0, 2.0f );
		DrawDebugSphere( GetWorld(), Start, 10.0f, 8, Color, false, 0.2f );
	}
}

AAuraPlayerController::AAuraPlayerController()
{
	// To allow our client version of player controller to control the server version of our player controller
	bReplicates = true;

	AutoMoveSpline = CreateDefaultSubobject<USplineComponent>( "AutoMoveSpline" );
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
	AuraInputComponent->BindAction( MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::MoveWithButtons );
	check( ShiftAction );
	AuraInputComponent->BindAction( ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed );
	AuraInputComponent->BindAction( ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased );
	AuraInputComponent->BindAbilityActions( InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld );
}
void AAuraPlayerController::AbilityInputTagPressed( FGameplayTag InputTag )
{
	// Don't call this shit each frame. If you need to add such check in tick function
	// then subscribe to EGameplayTagEventType::NewOrRemoved and change bool field in the class
	if ( GetASC() && GetASC()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputPressed ) )
	{
		return;
	}

	if ( InputTag.MatchesTagExact( FAuraGameplayTags::Get().AuraInput_LMB ) )
	{
		bAutoMove = false;
	}

	if ( IsValid( GetASC() ) )
	{
		GetASC()->AbilityInputTagPressed( InputTag );
	}
}
void AAuraPlayerController::AbilityInputTagReleased( FGameplayTag InputTag )
{
	// Don't call this shit each frame. If you need to add such check in tick function
	// then subscribe to EGameplayTagEventType::NewOrRemoved and change bool field in the class
	if ( GetASC() && GetASC()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputReleased ) )
	{
		return;
	}

	// Move character if LMB was clicked and no actors under cursor
	bool bShootingMode = bTargeting || bShiftPressed;
	bool bIsClick = FollowTime <= HoldButtonThreshold;
	if ( InputTag.MatchesTagExact( FAuraGameplayTags::Get().AuraInput_LMB ) && !bShootingMode && bIsClick )
	{
		GeneratePathToPoint( LastCursorTraceImpactPoint );
		// Don't call this shit each frame. If you need to add such check in tick function
		// then subscribe to EGameplayTagEventType::NewOrRemoved and change bool field in the class
		if ( GetASC() && !GetASC()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputPressed ) )
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation( this, ClickNS, LastCursorTraceImpactPoint );
		}
	}
	// Activate\Diactivate ability otherwise
	else
	{
		bAutoMove = false;
		if ( IsValid( GetASC() ) )
		{
			GetASC()->AbilityInputTagReleased( InputTag );
		}
	}
	FollowTime = 0.f;
}
void AAuraPlayerController::AbilityInputTagHeld( FGameplayTag InputTag )
{
	// Don't call this shit each frame. If you need to add such check in tick function
	// then subscribe to EGameplayTagEventType::NewOrRemoved and change bool field in the class
	if ( GetASC() && GetASC()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_InputHeld ) )
	{
		return;
	}

	// Move character if LMB pressed and no actors under cursor
	bool bShootingMode = bTargeting || bShiftPressed;
	if ( InputTag.MatchesTagExact( FAuraGameplayTags::Get().AuraInput_LMB ) && !bShootingMode )
	{
		MoveWithCursor();
	}
	// Activate ability otherwise
	else
	{
		if ( IsValid( GetASC() ) )
		{
			GetASC()->AbilityInputTagHeld( InputTag );
		}
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation( float Damage, ACharacter* TargetCharacter, bool bIsBlockedHit, bool bIsCriticalHit ) const
{
	check( DamageTextComponentClass );
	if ( IsValid( TargetCharacter ) )
	{
		// For Outer I could use "this" and it would make sense as it's a UI part of the player
		// But I decided to use TargetCharacter as another precaution if I'll mess up with lifespan of the widgetcomponent
		// If so, the widgetcomponent will die and be destroyed with the enemy character
		UDamageTextComponent* FloatingDamageText = NewObject<UDamageTextComponent>( TargetCharacter, DamageTextComponentClass );
		// Registering component when creating it not in constructor
		FloatingDamageText->RegisterComponent();
		// The best way is to use SetWorldLocation + SocketLocation of the characters socket under the head
		FloatingDamageText->SetWorldLocation( TargetCharacter->GetActorLocation() );
		// Another way to set location is to attach and detach. But the method above is a prefferable
		// FloatingDamageText->AttachToComponent( TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform );
		// FloatingDamageText->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
		EDamageTextType DamageTextType = EDamageTextType::Normal;

		if ( bIsBlockedHit && bIsCriticalHit )
		{
			DamageTextType = EDamageTextType::BlockedCritical;
		}
		else if ( bIsBlockedHit )
		{
			DamageTextType = EDamageTextType::Blocked;
		}
		else if ( bIsCriticalHit )
		{
			DamageTextType = EDamageTextType::Critical;
		}

		FloatingDamageText->SetDamageText( Damage, DamageTextType );
	}
}

void AAuraPlayerController::MoveWithButtons( const FInputActionValue& InputActionValue )
{
	bAutoMove = false;

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
void AAuraPlayerController::MoveWithCursor()
{
	APawn* ControlledPawn = GetPawn<APawn>();
	if ( !IsValid( ControlledPawn ) )
	{
		return;
	}

	bAutoMove = false;

	FollowTime += GetWorld()->GetDeltaSeconds();
	AutoMoveDestinationPoint = LastCursorTraceImpactPoint;
	AutoMoveDirection = ( AutoMoveDestinationPoint - ControlledPawn->GetActorLocation() ).GetSafeNormal();

	ControlledPawn->AddMovementInput( AutoMoveDirection, 1.f );
}

void AAuraPlayerController::GeneratePathToPoint( FVector TargetPoint )
{
	APawn* ControlledPawn = GetPawn<APawn>();
	if ( !IsValid( ControlledPawn ) )
	{
		return;
	}

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously( this, ControlledPawn->GetActorLocation(), LastCursorTraceImpactPoint );
	if ( !NavPath )
	{
		return;
	}

	AutoMoveSpline->ClearSplinePoints( false );
	for ( FVector NavPoint : NavPath->PathPoints )
	{
		AutoMoveSpline->AddSplinePoint( NavPoint, ESplineCoordinateSpace::World, false );
		DrawDebugSphere( GetWorld(), NavPoint, 8.f, 8, FColor::Green, false, 2.f );
	}
	AutoMoveSpline->UpdateSpline();

	// In case if a player pressed to unaccessable spot
	if ( !NavPath->PathPoints.IsEmpty() )
	{
		AutoMoveDestinationPoint = NavPath->PathPoints.Last();
		bAutoMove = true;
	}
}

void AAuraPlayerController::AutoMoveAlongMovementSpline()
{
	APawn* ControlledPawn = GetPawn();
	if ( IsValid( ControlledPawn ) )
	{
		// Spline has direction. So here we find closest point to the character and move character along the spline using direction from the closest point to the next point in spline
		const FVector LocationOnSpline = AutoMoveSpline->FindLocationClosestToWorldLocation( ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World );
		const FVector Direction = AutoMoveSpline->FindDirectionClosestToWorldLocation( LocationOnSpline, ESplineCoordinateSpace::World );
		ControlledPawn->AddMovementInput( Direction );

		const float DistanceToDestination = ( LocationOnSpline - AutoMoveDestinationPoint ).Length();
		if ( DistanceToDestination <= AutoMoveDisableDistanceThreshold )
		{
			bAutoMove = false;
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	// Don't call this shit each frame. If you need to add such check in tick function
	// then subscribe to EGameplayTagEventType::NewOrRemoved and change bool field in the class
	if ( GetPawn() && GetASC() && GetASC()->HasMatchingGameplayTag( FAuraGameplayTags::Get().Player_Block_CursorTrace ) )
	{
		if ( LastActorUnderCursorToHighlight ) LastActorUnderCursorToHighlight->UnHighlightActor();
		if ( CurrentActorUnderCursorToHighlight ) CurrentActorUnderCursorToHighlight->UnHighlightActor();
		LastActorUnderCursorToHighlight = nullptr;
		CurrentActorUnderCursorToHighlight = nullptr;
		bTargeting = false;
		return;
	}

	FHitResult Hit;
	GetHitResultUnderCursor( ECC_Visibility, false, Hit );
	if ( !Hit.bBlockingHit )
	{
		return;
	}

	LastCursorTraceImpactPoint = Hit.ImpactPoint;
	ActorUnderCursor = Hit.GetActor();

	UpdateHightlightActor();
}
void AAuraPlayerController::UpdateHightlightActor()
{
	LastActorUnderCursorToHighlight = CurrentActorUnderCursorToHighlight;
	CurrentActorUnderCursorToHighlight = ActorUnderCursor;

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
	if ( CurrentActorUnderCursorToHighlight == LastActorUnderCursorToHighlight )
	{
		return;
	}

	// The order is important to correctly process case when we hightlight new actor and at the same time unhighlight previous
	// So we should first make bTargeting false and then true and not vice versa
	// If there was a previously highlighted actor, unhighlight it
	if ( LastActorUnderCursorToHighlight )
	{
		LastActorUnderCursorToHighlight->UnHighlightActor();
		bTargeting = false;
	}

	// If there is a new actor to highlight, highlight it
	if ( CurrentActorUnderCursorToHighlight )
	{
		CurrentActorUnderCursorToHighlight->HighlightActor();
		bTargeting = true;
	}
}
void AAuraPlayerController::PlayerTick( float DeltaSeconds )
{
	Super::PlayerTick( DeltaSeconds );
	CursorTrace();

	if ( bAutoMove )
	{
		AutoMoveAlongMovementSpline();
	}
}
