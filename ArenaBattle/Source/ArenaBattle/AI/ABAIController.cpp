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
	// Blackboard는 AAIController에서 기본적으로 제공하는 멤버 변수
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	if (UseBlackboard(BlackboardData, BlackboardComponent) == true)
	{
		// Blackboard의 InitPos key값을 AIController가 빙의한 Pawn의 위치로 초기화
		Blackboard->SetValueAsVector(TEXT("InitPos"), GetPawn()->GetActorLocation());

		bool bResult = RunBehaviorTree(BehaviorTree);
		ensure(bResult == true);
	}
}

void AABAIController::StopAI()
{
	// BrainComponent는 AAIController에서 기본적으로 제공하는 멤버 변수
	// BehaviorTreeComponent 외에도 다른 클래스가 AI의 Brain 역할을 할 수 있기 때문에
	// UBrainComponent 형식으로 들고 있음
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

// Controller가 Pawn에 빙의할 때 호출되는 콜백 함수
void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
