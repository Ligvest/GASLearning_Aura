// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraPlayerCharacter.generated.h"

class AAuraPlayerState;
class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// ~Begin of IPlayerInterface
	virtual int32 FindLevelForXP_Implementation( int32 InXP ) const override;
	virtual void AddToXP_Implementation( int32 XpToAdd ) override;
	virtual int32 GetXP_Implementation() const override;
	virtual void AddToPlayerLevel_Implementation( int32 LevelToAdd ) override;
	virtual void LevelUp_Implementation() override;
	virtual void AddToAttributePoints_Implementation( int32 InAttributePoints ) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetAttributePointsReward_Implementation( int32 Level ) const override;
	virtual void AddToSpellPoints_Implementation( int32 InSpellPoints ) override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual int32 GetSpellPointsReward_Implementation( int32 Level ) const override;
	virtual void ShowMagicCircle_Implementation( UMaterialInterface* DecalMaterial ) override;
	virtual void HideMagicCircle_Implementation() override;
	// ~End of IPlayerInterface

	AAuraPlayerCharacter();
	virtual void BeginPlay() override;
	virtual void PossessedBy( AController* NewController ) override;

	// Replication
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;

protected:
	AAuraPlayerState* AuraPS;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USpringArmComponent> CameraBoom;

private:
	void InitGASInfo();
	void InitHUD() const;
	void InitGASInfoAndHUD();

	UFUNCTION( NetMulticast, Reliable )
	void MulticastLevelUpParticles() const;
};
