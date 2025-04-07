#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()

public:
	AABAIController();

public:
	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BlackboardData;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BehaviorTree;
};
