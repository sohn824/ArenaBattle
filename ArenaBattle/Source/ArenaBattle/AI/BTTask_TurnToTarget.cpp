#include "AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (TargetPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AICharacterInterface = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (AICharacterInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	float TurnSpeed = AICharacterInterface->GetAITurnSpeed();
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRotator = FRotationMatrix::MakeFromX(LookVector).Rotator();
	// ControllingPawn을 현재 회전값에서 목표 회전값까지 보간해서 서서히 회전시킨다
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(),
		TargetRotator, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
