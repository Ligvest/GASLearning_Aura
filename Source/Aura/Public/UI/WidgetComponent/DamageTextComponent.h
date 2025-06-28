// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

UENUM( BlueprintType )
enum class EDamageTextType : uint8
{
	Normal UMETA( DisplayName = "Normal" ),
	Blocked UMETA( DisplayName = "Blocked" ),
	Critical UMETA( DisplayName = "Critical" ),
	BlockedCritical UMETA( DisplayName = "BlockedCritical" )
};

/**
 *
 */
UCLASS()
class AURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintImplementableEvent )
	void SetDamageText( float Damage, const EDamageTextType DamageTextType );
};
