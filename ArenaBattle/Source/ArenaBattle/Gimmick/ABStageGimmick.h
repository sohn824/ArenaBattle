#pragma once

/*
* 스테이지 관련된 기능들을 모아놓은 클래스
*/

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

UENUM(BlueprintType)
enum class EStageState : uint8
{
	STATE_READY = 0,
	STATE_FIGHT,
	STATE_REWARD,
	STATE_NEXT,
};

// Stage의 State가 변경됐을 때 호출할 콜백함수 Delegate 선언
DECLARE_DELEGATE(FOnSetStageStateDelegate);
// Delegate를 배열에 담아 관리하기 위해 구조체로 감싼다.
USTRUCT(BlueprintType)
struct FOnSetStageStateDelegateWrapper
{
	GENERATED_BODY()
	FOnSetStageStateDelegateWrapper() {}
	FOnSetStageStateDelegateWrapper(const FOnSetStageStateDelegate& InDelegate) : Delegate(InDelegate) {}

	FOnSetStageStateDelegate Delegate;
};

UCLASS()
class ARENABATTLE_API AABStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	AABStageGimmick();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

/// Stage Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> StageMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTriggerBox;

	// UBoxComponent의 Overlap 이벤트에 bind해줄 함수
	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

/// Gate Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> GateMeshComponentMap;
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggerBoxArray;

	// UBoxComponent의 Overlap 이벤트에 bind 해줄 함수
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	void OpenAllGates();
	void CloseAllGates();

/// State Section
protected:
	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;
public:
	void SetState(EStageState NewState);

protected:
	UPROPERTY()
	TMap<EStageState, FOnSetStageStateDelegateWrapper> OnSetStateDelegateMap;

	void OnSetState_Ready();
	void OnSetState_Fight();
	void OnSetState_Reward();
	void OnSetState_Next();

/// STATE_FIGHT
protected:
	// 적이 될 수 있는 클래스는 AABCharacterNonPlayer를 기반으로 하는 클래스로 한정함
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABCharacterNonPlayer> EnemyClass;
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float EnemySpawnFirstDelay;

	UFUNCTION()
	void SpawnEnemy();
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
	FTimerHandle EnemySpawnDelayTimerHandle;

/// STATE_REWARD
protected:
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABItemBox> ItemBoxClass;
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class AABItemBox>> ItemBoxes;

	TMap<FName, FVector> ItemBoxLocationMap;

	UFUNCTION()
	void SpawnItemBoxes();
	// UBoxComponent의 Overlap 이벤트에 bind 해줄 함수
	UFUNCTION()
	void OnItemBoxTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
};
