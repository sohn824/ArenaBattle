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

	// �ִϸ��̼� ��Ÿ�ְ� ������ ���� �� Task ���� ������ �ϵ��� �Ѵ�
	FOnAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AICharacterInterface->SetAICharacterAttackFinishedDelegate(OnAttackFinished);

	AICharacterInterface->AttackByAI();

	// �ִϸ��̼� ��Ÿ�ְ� ������ ������ Task ������ �ƴ϶� ���� ������ ����
	return EBTNodeResult::InProgress;
}
