// Dovzhik Tolya

#include "UI/Widget/AuraUserWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraWidgetController.h"

void UAuraUserWidget::SetWidgetController( UObject* InWidgetController )
{
	// The base class of WidgetController is UAuraWidgetController, so it always should be at least of this type
	// Not sure why Stephan decided to use UObject everyhwhere instead of UAuraWidgetController
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
void UAuraUserWidget::Open( UUserWidget* InParentWidget, UButton* OpenButton )
{
	if ( InParentWidget )
	{
		ParentWidget = InParentWidget;
		InParentWidget->SetIsEnabled( false );
	}

	if ( OpenButton )
	{
		ParentButton = OpenButton;
		ParentButton->SetIsEnabled( false );
	}

	// Disable input for character
	{
		APlayerController* PC = GetOwningPlayer();
		check( PC );
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior( EMouseLockMode::DoNotLock );
		// InputMode.SetWidgetToFocus( TakeWidget() );
		PC->SetInputMode( InputMode );
	}

	AddToViewport();
}
void UAuraUserWidget::Close( bool ReturnInputToCharacter )
{
	if ( ParentWidget )
	{
		ParentWidget->SetIsEnabled( true );
	}

	if ( ParentButton )
	{
		ParentButton->SetIsEnabled( true );
	}

	// Return input to character
	// If I want to open several windows then I should make a bool for each window and check it here I believe.
	// Or make good and beautiful stack ;)
	if ( ReturnInputToCharacter )
	{
		APlayerController* PC = GetOwningPlayer();
		check( PC );
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior( EMouseLockMode::DoNotLock );
		InputMode.SetHideCursorDuringCapture( false );
		// if ( ParentWidget )
		// {
		// 	InputMode.SetWidgetToFocus( ParentWidget->TakeWidget() );
		// }
		PC->SetInputMode( InputMode );
	}

	OnWidgetClosed.Broadcast();

	RemoveFromParent();
}