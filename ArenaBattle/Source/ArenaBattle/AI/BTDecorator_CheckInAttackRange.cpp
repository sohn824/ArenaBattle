#include "AI/BTDecorator_CheckInAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"

UBTDecorator_CheckInAttackRange::UBTDecorator_CheckInAttackRange()
{
	NodeName = TEXT("CheckInAttackRange");
}

bool UBTDecorator_CheckInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	IABCharacterAIInterface* AICharacterInterface = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (AICharacterInterface == nullptr)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRange = AICharacterInterface->GetAIAttackRange();
	if (DistanceToTarget > AttackRange)
	{
		return false;
	}

	return true;
}
