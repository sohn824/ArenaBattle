#pragma once

/*
* �������� ���õ� ��ɵ��� ��Ƴ��� Ŭ����
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

// Stage�� State�� ������� �� ȣ���� �ݹ��Լ� Delegate ����
DECLARE_DELEGATE(FOnSetStageStateDelegate);
// Delegate�� �迭�� ��� �����ϱ� ���� ����ü�� ���Ѵ�.
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

	// UBoxComponent�� Overlap �̺�Ʈ�� bind���� �Լ�
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

	// UBoxComponent�� Overlap �̺�Ʈ�� bind ���� �Լ�
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
	// ���� �� �� �ִ� Ŭ������ AABCharacterNonPlayer�� ������� �ϴ� Ŭ������ ������
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
	// UBoxComponent�� Overlap �̺�Ʈ�� bind ���� �Լ�
	UFUNCTION()
	void OnItemBoxTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
};
