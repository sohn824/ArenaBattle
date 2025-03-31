#include "ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// SkeletalMesh가 로딩이 완료될 때까지 숨김
	GetMesh()->SetHiddenInGame(true);
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
