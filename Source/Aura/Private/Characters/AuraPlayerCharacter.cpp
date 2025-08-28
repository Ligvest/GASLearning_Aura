// Dovzhik Tolya

#include "Characters/AuraPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraGasBpLibrary.h"
#include "GAS/Data/AuraLevelUpInfo_DA.h"
#include "GAS/Debuff/DebuffNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

int32 AAuraPlayerCharacter::FindLevelForXP_Implementation( int32 InXP ) const
{
	check( AuraPS );
	return AuraPS->LevelUpInfo_DA->FindLevelForXp( InXP );
}

void AAuraPlayerCharacter::AddToXP_Implementation( int32 XpToAdd )
{
	check( AuraPS );
	AuraPS->AddToXP( XpToAdd );
}

int32 AAuraPlayerCharacter::GetXP_Implementation() const
{
	check( AuraPS );
	return AuraPS->GetXP();
}

void AAuraPlayerCharacter::AddToPlayerLevel_Implementation( int32 LevelToAdd )
{
	check( AuraPS );
	AuraPS->AddToLevel( LevelToAdd );

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>( GetAbilitySystemComponent() );
	check( AuraASC );
	AuraASC->UpdateAbilityStatuses( AuraPS->GetPlayerLevel() );
}

void AAuraPlayerCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AAuraPlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if ( IsValid( LevelUpNiagaraComponent ) )
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = ( CameraLocation - NiagaraSystemLocation ).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation( ToCameraRotation );
		LevelUpNiagaraComponent->Activate( true );
	}
}

void AAuraPlayerCharacter::AddToAttributePoints_Implementation( int32 InAttributePoints )
{
	check( AuraPS );
	AuraPS->AddToAttributePoints( InAttributePoints );
}

int32 AAuraPlayerCharacter::GetAttributePoints_Implementation() const
{
	check( AuraPS );
	return AuraPS->GetPlayerAttributePoints();
}

int32 AAuraPlayerCharacter::GetSpellPoints_Implementation() const
{
	check( AuraPS );
	return AuraPS->GetPlayerSpellPoints();
}

int32 AAuraPlayerCharacter::GetAttributePointsReward_Implementation( int32 Level ) const
{
	check( AuraPS );
	FAuraLevelUpInfo LevelUpInfo = AuraPS->LevelUpInfo_DA->FindLevelUpInfoForLevel( Level );
	return LevelUpInfo.AttributePointsReward;
}

void AAuraPlayerCharacter::AddToSpellPoints_Implementation( int32 InSpellPoints )
{
	check( AuraPS );
	AuraPS->AddToSpellPoints( InSpellPoints );
}

int32 AAuraPlayerCharacter::GetSpellPointsReward_Implementation( int32 Level ) const
{
	check( AuraPS );
	FAuraLevelUpInfo LevelUpInfo = AuraPS->LevelUpInfo_DA->FindLevelUpInfoForLevel( Level );
	return LevelUpInfo.SpellPointsReward;
}

void AAuraPlayerCharacter::ShowMagicCircle_Implementation( UMaterialInterface* DecalMaterial )
{
	if ( AAuraPlayerController* PC = GetController<AAuraPlayerController>() )
	{
		PC->ShowMagicCircle( DecalMaterial );
	}
}

void AAuraPlayerCharacter::HideMagicCircle_Implementation()
{
	if ( AAuraPlayerController* PC = GetController<AAuraPlayerController>() )
	{
		PC->HideMagicCircle();
	}
}

AAuraPlayerCharacter::AAuraPlayerCharacter()
{
	CharacterClass = ECharacterClass::Player;

	Tags.Add( UAuraGasBpLibrary::GetPlayerActorTag() );

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( "CameraBoom" );
	CameraBoom->SetupAttachment( GetRootComponent() );
	CameraBoom->SetUsingAbsoluteRotation( true );
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>( "TopDownCameraComponent" );
	TopDownCameraComponent->SetupAttachment( CameraBoom, USpringArmComponent::SocketName );
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>( "LevelUpNiagaraComponent" );
	LevelUpNiagaraComponent->SetupAttachment( GetRootComponent() );
	LevelUpNiagaraComponent->bAutoActivate = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	check( MovementComponent );

	// Turn character in the direction of the movement
	MovementComponent->bOrientRotationToMovement = true;
	// Character turn speed
	MovementComponent->RotationRate = FRotator( 0.0f, 400.0f, 0.0f );

	// Allow movement only by X and Y axis ( restrict Z axis )
	// This is default axis. If you want to make 2D game then you can restrict X axis instead
	MovementComponent->SetPlaneConstraintNormal( FVector( 0, 0, 1 ) );
	MovementComponent->bConstrainToPlane = true;
	// Snap to XY plane on start ( so if your character will be spawned higher it won't stuck there)
	MovementComponent->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Server
void AAuraPlayerCharacter::PossessedBy( AController* NewController )
{
	Super::PossessedBy( NewController );

	AuraPS = CastChecked<AAuraPlayerState>( GetPlayerState() );

	// For initializing AbilityComponent we use PossessedBy because without controlling the character the abilities don't make sense
	// Init ability actor info for Server
	InitGASInfoAndHUD();
	GrantDefaultAbilities();
}

// Client
void AAuraPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AuraPS = CastChecked<AAuraPlayerState>( GetPlayerState() );

	// For initializing AbilityComponent we use OnRep_PlayerState instead of AknowledgePossession on a client
	// because we need to be sure that PlayerState is replicated and has all valid data so we can use it
	// Also this ASC is stored in PlayerState so without correct ASC we can't initialize the rest of GAS
	// Init ability actor info for Client
	InitGASInfoAndHUD();
}

void AAuraPlayerCharacter::OnRep_Stunned()
{
	if ( UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent ) )
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag( GameplayTags.Player_Block_CursorTrace );
		BlockedTags.AddTag( GameplayTags.Player_Block_InputHeld );
		BlockedTags.AddTag( GameplayTags.Player_Block_InputPressed );
		BlockedTags.AddTag( GameplayTags.Player_Block_InputReleased );
		if ( bIsStunned )
		{
			// This is stupid hack to De\Activate here as we are creating dynamic GE in PostGameplayEffectExecute
			// And due to this it doesn't replicate granted tags. For more info see the last comment in PostGameplayEffectExecute
			AuraASC->AddLooseGameplayTags( BlockedTags );
			StunDebuffNSComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags( BlockedTags );
			StunDebuffNSComponent->Deactivate();
		}
	}
}

void AAuraPlayerCharacter::OnRep_Burned()
{
	Super::OnRep_Burned();
	// This is stupid hack to De\Activate here as we are creating dynamic GE in PostGameplayEffectExecute
	// And due to this it doesn't replicate granted tags. For more info see the last comment in PostGameplayEffectExecute
	if ( bIsBurned )
	{
		BurnDebuffNSComponent->Activate();
	}
	else
	{
		BurnDebuffNSComponent->Deactivate();
	}
}

void AAuraPlayerCharacter::InitGASInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo( AuraPlayerState, this );
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	check( AuraASC );
	AuraASC->Init();
	OnAscRegisteredDelegate.Broadcast( AbilitySystemComponent );
	AbilitySystemComponent->RegisterGameplayTagEvent( FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved ).AddUObject( this, &AAuraPlayerCharacter::StunTagChanged );
	AttributeSet = AuraPlayerState->GetAttributeSet();
	// It can be called only on the server as attributes will be replicated regardless
	InitDefaultAttributes( GetCharacterLevel() );
}
void AAuraPlayerCharacter::InitHUD() const
{
	const APlayerController* PC = Cast<APlayerController>( GetController() );
	AAuraHUD* HUD = PC->GetHUD<AAuraHUD>();
	check( HUD );
	HUD->InitHUDWidget();
}
void AAuraPlayerCharacter::InitGASInfoAndHUD()
{
	InitGASInfo();

	// Init HUD only if this is a client. And this client controls this character ( so that it has valid PlayerController )
	if ( IsLocallyControlled() )
	{
		InitHUD();
	}
}
