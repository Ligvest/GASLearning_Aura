// Dovzhik Tolya

#include "UI/Widget/AuraUserWidget.h"

#include "UI/WidgetController/AuraWidgetController.h"

void UAuraUserWidget::SetWidgetController( UObject* InWidgetController )
{
	// The base class of WidgetController is UAuraWidgetController, so it always should be at least of this type
	// Not sure why Stephan decided to use UObject everyhwhere instead of UAuraWidgetController
	WidgetController = InWidgetController;
	WidgetControllerSet();
}