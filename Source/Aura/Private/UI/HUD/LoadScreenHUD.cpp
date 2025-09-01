// Dovzhik Tolya

#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>( this, LoadScreenViewModelClass );
	// Create MVVMs for WidgetSwitchers inside Root LoadScreen widgets MVVM
	LoadScreenViewModel->InitializeLoadSlots();

	LoadScreenWidget = CreateWidget<ULoadScreenWidget>( GetWorld(), LoadScreenWidgetClass );
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();
}