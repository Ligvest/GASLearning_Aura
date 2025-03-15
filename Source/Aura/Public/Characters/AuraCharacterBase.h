// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	UPROPERTY( EditDefaultsOnly, Category = SocketNames )
	FName SocketNameHandWeapon = FName( "HandWeapon" );

	UPROPERTY( EditAnywhere, Category = Combat )
	TObjectPtr<class USkeletalMeshComponent> WeaponMeshComponent;

protected:
	virtual void BeginPlay() override;
};