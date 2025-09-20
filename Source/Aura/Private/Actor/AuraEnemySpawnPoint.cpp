// Dovzhik Tolya

#include "Actor/AuraEnemySpawnPoint.h"

#include "Characters/AuraEnemyCharacter.h"

void AAuraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAuraEnemyCharacter* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemyCharacter>( EnemyClass, GetActorTransform() );
	Enemy->SetLevel( EnemyLevel );
	Enemy->SetCharacterClass( CharacterClass );
	Enemy->SpawnDefaultController();
	Enemy->FinishSpawning( GetActorTransform() );
}
