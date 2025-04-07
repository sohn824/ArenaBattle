#include "AI/ABAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AABAIController::AABAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardDataRef(
		TEXT("/Script/AIModule.BlackboardData'/Game/ArenaBattle/AI/BB_ABCharacter.BB_ABCharacter'"));
	if (BlackboardDataRef.Object)
	{
		BlackboardData = BlackboardDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(
		TEXT("/Script/AIModule.BehaviorTree'/Game/ArenaBattle/AI/BT_ABCharacter.BT_ABCharacter'"));
	if (BehaviorTreeRef.Object)
	{
		BehaviorTree = BehaviorTreeRef.Object;
	}
}

void AABAIController::RunAI()
{
	// Blackboard�� AAIController���� �⺻������ �����ϴ� ��� ����
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	if (UseBlackboard(BlackboardData, BlackboardComponent) == true)
	{
		// Blackboard�� InitPos key���� AIController�� ������ Pawn�� ��ġ�� �ʱ�ȭ
		Blackboard->SetValueAsVector(TEXT("InitPos"), GetPawn()->GetActorLocation());

		bool bResult = RunBehaviorTree(BehaviorTree);
		ensure(bResult == true);
	}
}

void AABAIController::StopAI()
{
	// BrainComponent�� AAIController���� �⺻������ �����ϴ� ��� ����
	// BehaviorTreeComponent �ܿ��� �ٸ� Ŭ������ AI�� Brain ������ �� �� �ֱ� ������
	// UBrainComponent �������� ��� ����
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

// Controller�� Pawn�� ������ �� ȣ��Ǵ� �ݹ� �Լ�
void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
