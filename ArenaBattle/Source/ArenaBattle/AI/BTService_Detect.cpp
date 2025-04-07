#include "AI/BTService_Detect.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

UBTService_Detect::UBTService_Detect()
{
	// BT���� ����� Service ��� �̸�
	NodeName = TEXT("Detect");
	// üũ ����
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	IABCharacterAIInterface* AICharacterInterface = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (AICharacterInterface == nullptr)
	{
		return;
	}

	FVector CenterPos = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	float DetectRange = AICharacterInterface->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	// @ InTraceTag
	// @ bInTraceComplex
	// @ InIgnoreActor
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		CenterPos,
		FQuat::Identity,
		CCHANNEL_ABACTION, //ECC_GameTraceChannel1
		FCollisionShape::MakeSphere(DetectRange),
		CollisionQueryParams
	);

	bool bAttackSucceed = false;

	if (bResult == true)
	{
		for (FOverlapResult const& OverlapResult : OverlapResults)
		{
			APawn* OverlapPawn = Cast<APawn>(OverlapResult.GetActor());
			// ���� ĳ���͸� ������ ���
			if (OverlapPawn != nullptr && OverlapPawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), OverlapPawn);

				DrawDebugSphere(World, CenterPos, DetectRange, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, OverlapPawn->GetActorLocation(), 10.f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), OverlapPawn->GetActorLocation(),
					FColor::Green, false, 0.2f);

				bAttackSucceed = true;
			}
		}
	}

	// �浹 ������ ������Ʈ�� ���� ���
	if (bAttackSucceed == false)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
		DrawDebugSphere(World, CenterPos, DetectRange, 16, FColor::Red, false, 0.2f);
	}
}
