// Dovzhik Tolya

#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check( GEngine );
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>( GEngine->AssetManager );
	check( AuraAssetManager );

	return *AuraAssetManager;
}
void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAuraGameplayTags::Get().InitializeNativeGameplayTags();
}