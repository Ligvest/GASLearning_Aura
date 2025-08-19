// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AuraAICharacter.h"
#include "GAS/Data/AuraCharacterClassInfoDA.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemyCharacter.generated.h"

class UWidgetComponent;
/**
 *
 */
UCLASS()
class AURA_API AAuraEnemyCharacter : public AAuraAICharacter, public IEnemyInterface
{
public:
	//~ Begin of IEnemyInterface
	virtual void SetCombatActor_Implementation( AActor* InCombatActor ) override;
	virtual AActor* GetCombatActor_Implementation() const override;
	//~ End of IEnemyInterface

	//~ Begin of ICombatInterface
	virtual int GetCharacterLevel() const override;
	//~ End of ICombatInterface

	AAuraEnemyCharacter();
	virtual void BeginPlay() override;

	UFUNCTION( BlueprintPure )
	UAuraEnemyOverlayWC* GetOverlayWC() const;

	virtual void PossessedBy( AController* NewController ) override;

	UFUNCTION( BlueprintPure )
	float GetDistanceToSeePlayer() const { return DistanceToSeePlayer; }

protected:
	AActor* CombatActor = nullptr;

	UPROPERTY()
	TObjectPtr<UAuraEnemyOverlayWC> FloatingWC;

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UAuraEnemyOverlayWC> FloatingWcClass;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UWidgetComponent> FloatingWidget;

	bool bHitReacting = false;

	// Death and Dissolve
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Death" )
	float CorpseLifeSpan = 5.f;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	int32 EnemyLevel = 1;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UPROPERTY( EditAnywhere, Category = AI )
	TObjectPtr<UBehaviorTree> BehaviorTree;

	const FName BBValueName_Ranged = FName( "RangedAttacker" );
	const FName BBValueName_UnderHitReaction = FName( "UnderHitReaction" );
	const FName BBValueName_Dead = FName( "Dead" );
	const FName BBValueName_DistanceToSeePlayer = FName( "DistanceToSeePlayer" );

	UPROPERTY( EditAnywhere, Category = AI )
	float DistanceToSeePlayer = 1000.f;

protected:
	UFUNCTION( BlueprintImplementableEvent )
	void SetupFloatingWidget();

	void InitFloatingWC();

	void InitReactionOnBeingHit();

	UFUNCTION()
	void ReactOnBeingHit( const FGameplayTag HitTag, const int NewTagCount );

	virtual void InitDefaultAttributes( int InCharacterLevel ) const override;

	virtual void StunTagChanged( const FGameplayTag CallbackTag, int32 NewCount ) override;

	// Death and Dissolve
	virtual void Die( FVector DeathImpulse ) override;

	bool IsRangedCharacter() const;

	GENERATED_BODY()
};
