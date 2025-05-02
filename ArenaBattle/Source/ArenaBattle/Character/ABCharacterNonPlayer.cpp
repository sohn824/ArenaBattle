#include "ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/ABAIController.h"
#include "CharacterStat/ABCharacterStatComponent.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// SkeletalMesh가 로딩이 완료될 때까지 숨김
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NpcMeshes.Num() > 0);

	int32 RandIdx = FMath::RandRange(0, NpcMeshes.Num() - 1);
	// SkeletalMesh 비동기 로드 시도
	NpcMeshLoadHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		NpcMeshes[RandIdx], FStreamableDelegate::CreateUObject(this, &AABCharacterNonPlayer::OnNpcMeshLoadCompleted));
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// 죽었을 경우 더 이상 BehaviorTree가 실행되지 않도록 멈춤
	AABAIController* ABAIController = Cast<AABAIController>(GetController());
	if (ABAIController)
	{
		ABAIController->StopAI();
	}

	// NonPlayer 캐릭터는 죽은 후 일정 시간이 지나면 사라지도록 함
	const float DestroyDelayTime = 5.f;
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
		FTimerDelegate::CreateLambda(
			[&]()
			{
				Destroy();
			}), DestroyDelayTime, false);
}

void AABCharacterNonPlayer::OnNpcMeshLoadCompleted()
{
	if (NpcMeshLoadHandle.IsValid())
	{
		USkeletalMesh* NpcMesh = Cast<USkeletalMesh>(NpcMeshLoadHandle->GetLoadedAsset());
		if (NpcMesh)
		{
			// SkeletalMesh가 로딩 완료되면 설정 후 숨김 해제
			GetMesh()->SetSkeletalMesh(NpcMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NpcMeshLoadHandle->ReleaseHandle();
}

float AABCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.f;
}

float AABCharacterNonPlayer::GetAIDetectRange()
{
	return 400.f;
}

float AABCharacterNonPlayer::GetAIAttackRange()
{
	return StatComponent->GetTotalStat().AttackRange + (StatComponent->GetAttackRadius() * 2);
}

float AABCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.f;
}

void AABCharacterNonPlayer::SetAICharacterAttackFinishedDelegate(const FOnAICharacterAttackFinished& InDelegate)
{
	OnAttackFinished = InDelegate;
}

void AABCharacterNonPlayer::AttackByAI()
{
	ComboActionProcess();
}

void AABCharacterNonPlayer::OnComboActionEnd()
{
	Super::OnComboActionEnd();

	// OnAttackFinished 델리게이트가 등록되어 있다면 추가로 호출해줌
	OnAttackFinished.ExecuteIfBound();
}
