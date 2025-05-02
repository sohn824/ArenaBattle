#include "ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/ABAIController.h"
#include "CharacterStat/ABCharacterStatComponent.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// SkeletalMesh�� �ε��� �Ϸ�� ������ ����
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NpcMeshes.Num() > 0);

	int32 RandIdx = FMath::RandRange(0, NpcMeshes.Num() - 1);
	// SkeletalMesh �񵿱� �ε� �õ�
	NpcMeshLoadHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		NpcMeshes[RandIdx], FStreamableDelegate::CreateUObject(this, &AABCharacterNonPlayer::OnNpcMeshLoadCompleted));
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// �׾��� ��� �� �̻� BehaviorTree�� ������� �ʵ��� ����
	AABAIController* ABAIController = Cast<AABAIController>(GetController());
	if (ABAIController)
	{
		ABAIController->StopAI();
	}

	// NonPlayer ĳ���ʹ� ���� �� ���� �ð��� ������ ��������� ��
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
			// SkeletalMesh�� �ε� �Ϸ�Ǹ� ���� �� ���� ����
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

	// OnAttackFinished ��������Ʈ�� ��ϵǾ� �ִٸ� �߰��� ȣ������
	OnAttackFinished.ExecuteIfBound();
}
