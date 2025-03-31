#include "ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// SkeletalMesh�� �ε��� �Ϸ�� ������ ����
	GetMesh()->SetHiddenInGame(true);
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
