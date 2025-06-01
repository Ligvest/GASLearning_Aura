// Dovzhik Tolya

#include "UI/Widget/AuraUserWidget.h"

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
void UAuraUserWidget::Open( UAuraUserWidget* InParentWidget, UButton* OpenButton )
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

	AddToViewport();
}
void UAuraUserWidget::Close()
{
	if ( ParentWidget )
	{
		ParentWidget->SetIsEnabled( true );
	}

	if ( ParentButton )
	{
		ParentButton->SetIsEnabled( true );
	}

	OnWidgetClosed.Broadcast();

	RemoveFromParent();
}