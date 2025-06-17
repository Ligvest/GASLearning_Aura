// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AuraAICharacter.h"
#include "GAS/Data/AuraCharacterClassInfoDA.h"
#include "AuraEnemyCharacter.generated.h"

class UWidgetComponent;
/**
 *
 */
UCLASS()
class AURA_API AAuraEnemyCharacter : public AAuraAICharacter
{
public:
	AAuraEnemyCharacter();
	virtual void BeginPlay() override;

	UFUNCTION( BlueprintPure )
	UAuraEnemyOverlayWC* GetOverlayWC() const;

protected:
	UPROPERTY()
	TObjectPtr<UAuraEnemyOverlayWC> FloatingWC;

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UAuraEnemyOverlayWC> FloatingWcClass;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UWidgetComponent> FloatingWidget;

	bool bHitReacting = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Movement" )
	float BaseMaxWalkSpeed = 250.f;

protected:
	UFUNCTION( BlueprintImplementableEvent )
	void SetupFloatingWidget();

	void InitFloatingWC();

	UPROPERTY( EditDefaultsOnly, Category = "CharacterDefaults" )
	ECharacterClass CharacterClass = ECharacterClass::Default;

	void InitReactionOnBeingHit();

	UFUNCTION()
	void ReactOnBeingHit( const FGameplayTag HitTag, const int NewTagCount );

	UPROPERTY( EditDefaultsOnly, Category = "Combat" )
	TSubclassOf<UGameplayAbility> HitReactAbilityClass;

	virtual void InitDefaultAttributes( int InCharacterLevel ) const override;

	GENERATED_BODY()
};
