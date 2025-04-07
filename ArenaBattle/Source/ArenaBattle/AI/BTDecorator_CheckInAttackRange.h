#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckInAttackRange.generated.h"

UCLASS()
class ARENABATTLE_API UBTDecorator_CheckInAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckInAttackRange();

protected:
	// BT Decorator의 만족 조건 설정 함수
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
