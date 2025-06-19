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

	// Death and Dissolve
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Death" )
	float CorpseLifeSpan = 5.f;

protected:
	UFUNCTION( BlueprintImplementableEvent )
	void SetupFloatingWidget();

	void InitFloatingWC();

	UPROPERTY( EditDefaultsOnly, Category = "CharacterDefaults" )
	ECharacterClass CharacterClass = ECharacterClass::Default;

	void InitReactionOnBeingHit();

	UFUNCTION()
	void ReactOnBeingHit( const FGameplayTag HitTag, const int NewTagCount );

	virtual void InitDefaultAttributes( int InCharacterLevel ) const override;

	// Death and Dissolve
	virtual void Die() override;

	GENERATED_BODY()
};
