// Dovzhik Tolya

#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>( "MagicCircleDecal" );
	MagicCircleDecal->SetupAttachment( GetRootComponent() );
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
}

void AMagicCircle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AMagicCircle::SetDecalMaterial( UMaterialInterface* NewDecalMaterial )
{
	MagicCircleDecal->SetMaterial( 0, NewDecalMaterial );
}
