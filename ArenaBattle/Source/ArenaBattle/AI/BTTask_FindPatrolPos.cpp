#include "AI/BTTask_FindPatrolPos.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{

}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AICharacterInterface = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (AICharacterInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector InitPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("InitPos"));

	FNavLocation NextPatrolPos;
	const float PatrolRadius = AICharacterInterface->GetAIPatrolRadius();
	if (NavSystem->GetRandomPointInNavigableRadius(InitPos, PatrolRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPos"), NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
