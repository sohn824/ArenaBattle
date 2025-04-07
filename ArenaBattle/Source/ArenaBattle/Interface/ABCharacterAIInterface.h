#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ABCharacterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UABCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FOnAICharacterAttackFinished);

class ARENABATTLE_API IABCharacterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void SetAICharacterAttackFinishedDelegate(const FOnAICharacterAttackFinished& InDelegate) = 0;
	virtual void AttackByAI() = 0;
};
