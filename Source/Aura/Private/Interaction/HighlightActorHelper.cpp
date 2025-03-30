// Dovzhik Tolya

#include "Interaction/HighlightActorHelper.h"

int FHighlightActorHelper::HighlightActorTypeToStencilValue( const EHighlightActorType HighlightType )
{
	// These values can be watched and changed in PP_Hightlight asset
	switch ( HighlightType )
	{
		case EHighlightActorType::Enemy:
			return 250;
		case EHighlightActorType::Ally:
			return 251;
		default:
			return 0;
	}

	// The code doesn't get here
	// return 0;
}