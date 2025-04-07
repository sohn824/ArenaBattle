#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AICharacterInterface = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (AICharacterInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 애니메이션 몽타주가 완전히 끝난 후 Task 성공 판정을 하도록 한다
	FOnAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AICharacterInterface->SetAICharacterAttackFinishedDelegate(OnAttackFinished);

	AICharacterInterface->AttackByAI();

	// 애니메이션 몽타주가 끝나기 전에는 Task 성공이 아니라 진행 중으로 본다
	return EBTNodeResult::InProgress;
}
