// Dovzhik Tolya

#include "Characters/AuraEnemyCharacter.h"

#include "AuraGameplayTags.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"
#include "GAS/AuraGasBpLibrary.h"
#include "GAS/Data/AuraCharacterClassInfoDA.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraEnemyOverlayWC.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"

//~ Begin of IEnemyInterface
void AAuraEnemyCharacter::SetCombatActor_Implementation( AActor* InCombatActor )
{
	CombatActor = InCombatActor;
}

AActor* AAuraEnemyCharacter::GetCombatActor_Implementation() const
{
	return CombatActor;
}
//~ End of IEnemyInterface

AAuraEnemyCharacter::AAuraEnemyCharacter()
{
	Tags.Add( UAuraGasBpLibrary::GetEnemyActorTag() );

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>( "AuraAbilitySystemComponent" );
	// #lig Replication
	AbilitySystemComponent->SetIsReplicated( true );
	AbilitySystemComponent->SetReplicationMode( EGameplayEffectReplicationMode::Minimal );

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>( "AuraAttributeSet" );

	FloatingWidget = CreateDefaultSubobject<UWidgetComponent>( TEXT( "OverlayWidget" ) );
	FloatingWidget->SetupAttachment( GetRootComponent() );
	FloatingWidget->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	// Movement
	// Disable rotation logic in pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// Enable rotation logic in MovementComponent where it could be smooth
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AAuraEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo( this, this );
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	check( AuraASC );
	AuraASC->Init();
	InitDefaultAttributes( CharacterLevel );
	check( AttributeSet );
	GetCharacterMovement()->MaxWalkSpeed = BaseMaxWalkSpeed;

	InitFloatingWC();
	check( FloatingWC );

	// Have additional function to setup widget controller for floating widget
	// We have it because we need a place to bind callback from WC to Widget
	// But we can't do this in BP begin play as it executes earlier than C++ begin play
	// And in this case we still not initialized our FloatingWC to use it in BeginPlay of BP
	SetupFloatingWidget();
	GrantDefaultAbilities();
	InitReactionOnBeingHit();
}

UAuraEnemyOverlayWC* AAuraEnemyCharacter::GetOverlayWC() const
{
	return FloatingWC;
}
void AAuraEnemyCharacter::PossessedBy( AController* NewController )
{
	Super::PossessedBy( NewController );

	if ( !HasAuthority() )
	{
		return;
	}
	AuraAIController = Cast<AAuraAIController>( NewController );
	UBlackboardComponent* BlackboardComponent = AuraAIController->GetBlackboardComponent();
	BlackboardComponent->InitializeBlackboard( *BehaviorTree->BlackboardAsset );
	BlackboardComponent->SetValueAsBool( BBValueName_Ranged, IsRangedCharacter() );
	BlackboardComponent->SetValueAsBool( BBValueName_UnderHitReaction, bHitReacting );
	BlackboardComponent->SetValueAsFloat( BBValueName_DistanceToSeePlayer, DistanceToSeePlayer );
	AuraAIController->RunBehaviorTree( BehaviorTree );
}

void AAuraEnemyCharacter::InitFloatingWC()
{
	FWidgetControllerParams WcParams;
	WcParams.AbilitySystemComponent = AbilitySystemComponent;
	WcParams.AttributeSet = AttributeSet;
	// These don't make sense for NPC
	WcParams.PlayerController = nullptr;
	WcParams.PlayerState = nullptr;

	check( FloatingWcClass );
	FloatingWC = NewObject<UAuraEnemyOverlayWC>( this, FloatingWcClass );
	FloatingWC->SetWidgetControllerParams( WcParams );
}

void AAuraEnemyCharacter::InitReactionOnBeingHit()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check( ASC );

	auto& OnHitReactTagToggledDelegate = ASC->RegisterGameplayTagEvent( FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved );
	OnHitReactTagToggledDelegate.AddUObject( this, &ThisClass::ReactOnBeingHit );
}

void AAuraEnemyCharacter::ReactOnBeingHit( const FGameplayTag HitTag, const int NewTagCount )
{
	if ( !HasAuthority() )
	{
		return;
	}

	bHitReacting = NewTagCount > 0;
	AuraAIController->GetBlackboardComponent()->SetValueAsBool( BBValueName_UnderHitReaction, bHitReacting );
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if ( bHitReacting )
	{
		MovementComponent->MaxWalkSpeed = 0.f;
	}
	else
	{
		MovementComponent->MaxWalkSpeed = BaseMaxWalkSpeed;
	}
}

void AAuraEnemyCharacter::Die()
{
	SetLifeSpan( CorpseLifeSpan );
	Super::Die();
}
bool AAuraEnemyCharacter::IsRangedCharacter() const
{
	return CharacterClass != ECharacterClass::Warrior;
}

void AAuraEnemyCharacter::InitDefaultAttributes( int InCharacterLevel ) const
{
	if ( !HasAuthority() )
	{
		return;
	}

	// GameMode available only on Server. Use GameState if you need to replicate GameMode specific data to clients
	AAuraGameModeBase* AuraGM = CastChecked<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) );
	UAuraCharacterClassInfoDA* DefaultCharacterInfoDA = AuraGM->GetDefaultCharacterInfoDA();

	// Should be set in defaults
	check( CharacterClass != ECharacterClass::Empty );
	TSubclassOf<UGameplayEffect> InitPrimaryAttribsEffectClass = DefaultCharacterInfoDA->GetClassDefaultInfo( CharacterClass ).InitPrimaryAttributesEffectClass;
	TSubclassOf<UGameplayEffect> InitSecondaryAttribsEffectClass = DefaultCharacterInfoDA->InitSecondaryAttributesEffectClass;
	TSubclassOf<UGameplayEffect> InitVitalAttribsEffectClass = DefaultCharacterInfoDA->InitVitalAttributesEffectClass;
	ApplyEffectToSelf( InitPrimaryAttribsEffectClass, InCharacterLevel );
	ApplyEffectToSelf( InitSecondaryAttribsEffectClass, InCharacterLevel );
	ApplyEffectToSelf( InitVitalAttribsEffectClass, InCharacterLevel );
}