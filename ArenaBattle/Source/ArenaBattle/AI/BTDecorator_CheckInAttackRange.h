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
	// BT Decorator�� ���� ���� ���� �Լ�
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
