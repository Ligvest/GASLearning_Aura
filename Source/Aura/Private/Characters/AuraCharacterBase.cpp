// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMeshComponent = CreateDefaultSubobject<class USkeletalMeshComponent>( "WeaponMeshComponent" );
	WeaponMeshComponent->SetupAttachment( GetMesh(), SocketNameHandWeapon );
	WeaponMeshComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}