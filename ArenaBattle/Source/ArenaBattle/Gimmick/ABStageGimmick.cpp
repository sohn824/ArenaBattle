#include "Gimmick/ABStageGimmick.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"
#include "Character/ABCharacterPlayer.h"
#include "Engine/OverlapResult.h"
#include "Item/ABItemBox.h"

AABStageGimmick::AABStageGimmick()
{
	// Init Stage Section
	StageMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMesh"));
	RootComponent = StageMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Object)
	{
		StageMeshComponent->SetStaticMesh(StageMeshRef.Object);
	}

	StageTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTriggerBox"));
	StageTriggerBox->SetBoxExtent(FVector(775.f, 775.f, 300.f));
	StageTriggerBox->SetupAttachment(StageMeshComponent);
	StageTriggerBox->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	StageTriggerBox->SetCollisionProfileName(CPROFILE_ABTRIGGER); // TEXT("ABTrigger")
	StageTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnStageTriggerBeginOverlap);

	// Init Gate Section
	static FName GateSocketNames[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));
	if (GateMeshRef.Object)
	{
		for (FName& GateSocketName : GateSocketNames)
		{
			// Gate Static Mesh Component
			UStaticMeshComponent* GateMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(GateSocketName);
			GateMeshComponent->SetStaticMesh(GateMeshRef.Object);
			GateMeshComponent->SetupAttachment(StageMeshComponent, GateSocketName);
			GateMeshComponent->SetRelativeLocation(FVector(0.f, -80.5f, 0.f));
			GateMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
			GateMeshComponentMap.Add(GateSocketName, GateMeshComponent);

			// Gate Trigger Box Component
			FName GateTriggerBoxName = *GateSocketName.ToString().Append(TEXT("TriggerBox"));
			UBoxComponent* GateTriggerBox = CreateDefaultSubobject<UBoxComponent>(GateTriggerBoxName);
			GateTriggerBox->SetBoxExtent(FVector(100.f, 100.f, 300.f));
			GateTriggerBox->SetupAttachment(StageMeshComponent, GateSocketName);
			GateTriggerBox->SetRelativeLocation(FVector(70.f, 0.f, 250.f));
			GateTriggerBox->SetCollisionProfileName(CPROFILE_ABTRIGGER); // TEXT("ABTrigger")
			GateTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);
			// ComponentTags - 그룹화를 위한 FName 태그 배열
			GateTriggerBox->ComponentTags.Add(GateSocketName);

			GateTriggerBoxArray.Add(GateTriggerBox);
		}
	}

	// Init State Section
	CurrentState = EStageState::STATE_READY;
	OnSetStateDelegateMap.Add(EStageState::STATE_READY,
		FOnSetStageStateDelegateWrapper(FOnSetStageStateDelegate::CreateUObject(this, &AABStageGimmick::OnSetState_Ready)));
	OnSetStateDelegateMap.Add(EStageState::STATE_FIGHT,
		FOnSetStageStateDelegateWrapper(FOnSetStageStateDelegate::CreateUObject(this, &AABStageGimmick::OnSetState_Fight)));
	OnSetStateDelegateMap.Add(EStageState::STATE_REWARD,
		FOnSetStageStateDelegateWrapper(FOnSetStageStateDelegate::CreateUObject(this, &AABStageGimmick::OnSetState_Reward)));
	OnSetStateDelegateMap.Add(EStageState::STATE_NEXT,
		FOnSetStageStateDelegateWrapper(FOnSetStageStateDelegate::CreateUObject(this, &AABStageGimmick::OnSetState_Next)));

	// STATE_FIGHT
	EnemyClass = AABCharacterNonPlayer::StaticClass();
	EnemySpawnFirstDelay = 2.f;

	// STATE_REWARD
	ItemBoxClass = AABItemBox::StaticClass();
	for (FName& SocketName : GateSocketNames)
	{
		FVector ItemBoxLocation = StageMeshComponent->GetSocketLocation(SocketName) / 2;
		ItemBoxLocationMap.Add(SocketName, ItemBoxLocation);
	}

	// Stage Stat
	CurrentStageNum = 0;
}

// Unreal Editor에서 값 변경이 있을 때 호출되는 디버깅용 콜백
void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepHitResult)
{
	// 캐릭터가 Stage에 들어와 TriggerBox에 감지되면 STATE_FIGHT로 이동
	SetState(EStageState::STATE_FIGHT);
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepHitResult)
{
	// OverlappedComponent - 충돌을 감지한 Component (여기서는 GateMeshComponent)
	// +XGate, -XGate, +YGate, -YGate 중 하나의 ComponentTag를 달아놨음
	if (OverlappedComponent->ComponentTags.Num() <= 0)
	{
		return;
	}

	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// SocketName은 +X, -X, +Y, -Y 중 하나
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	if (StageMeshComponent->DoesSocketExist(SocketName) == false)
	{
		return;
	}

	FVector NewLocation = StageMeshComponent->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;

	// FCollisonQueryParams - UE의 충돌 검사를 위해 필요한 옵션값들의 구조체
	// @ TraceTag - 디버깅용 태그
	// @ bInTraceComplex - 복잡한 충돌검사 사용 여부
	// @ InIgnoreActor - 충돌검사에서 제외할 Actor 포인터
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(GateTrigger), false, this);

	// 무한 맵 기믹을 위해 현재 Stage를 제외하고
	// 특정 반경 내에 오브젝트가 없는지 확인 후 없다면 Stage를 새로 생성함
	// (이미 Stage가 있는 영역일 수도 있으므로)
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllStaticObjects,
		FCollisionShape::MakeSphere(775.f),
		CollisionQueryParams);

	if (bResult == false)
	{
		FTransform NewTransform(NewLocation);
		AABStageGimmick* NewStageGimmick = GetWorld()->SpawnActorDeferred<AABStageGimmick>(
			AABStageGimmick::StaticClass(), NewTransform);

		if (NewStageGimmick)
		{
			NewStageGimmick->SetCurrentStageNum(CurrentStageNum + 1);
			NewStageGimmick->FinishSpawning(NewTransform);
		}
	}
}

void AABStageGimmick::OpenAllGates()
{
	for (const auto& It : GateMeshComponentMap)
	{
		UStaticMeshComponent* GateMeshComp = It.Value;
		if (GateMeshComp)
		{
			GateMeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		}
	}
}

void AABStageGimmick::CloseAllGates()
{
	for (const auto& It : GateMeshComponentMap)
	{
		UStaticMeshComponent* GateMeshComp = It.Value;
		if (GateMeshComp)
		{
			GateMeshComp->SetRelativeRotation(FRotator::ZeroRotator);
		}
	}
}

void AABStageGimmick::SetState(EStageState NewState)
{
	CurrentState = NewState;

	// State에 맞게 bind된 Delegate 호출해주기
	if (OnSetStateDelegateMap.Contains(NewState))
	{
		OnSetStateDelegateMap[NewState].Delegate.ExecuteIfBound();
	}
}

void AABStageGimmick::OnSetState_Ready()
{
	// Stage Trigger Box가 캐릭터를 감지 가능하도록 설정
	StageTriggerBox->SetCollisionProfileName(CPROFILE_ABTRIGGER); // TEXT("ABTrigger")
	// Gate Trigger Box는 캐릭터를 감지하지 못하도록 설정
	for (auto& It : GateTriggerBoxArray)
	{
		UBoxComponent* GateTriggerBox = It;
		if (GateTriggerBox)
		{
			GateTriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}

	OpenAllGates();
}

void AABStageGimmick::OnSetState_Fight()
{
	StageTriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto& It : GateTriggerBoxArray)
	{
		UBoxComponent* GateTriggerBox = It;
		if (GateTriggerBox)
		{
			GateTriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}

	CloseAllGates();

	// EnemySpawnFirstDelay 후 SpawnEnemy() 실행
	GetWorld()->GetTimerManager().SetTimer(EnemySpawnDelayTimerHandle,
		this, &AABStageGimmick::SpawnEnemy, EnemySpawnFirstDelay, false);
}

void AABStageGimmick::OnSetState_Reward()
{
	StageTriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto& It : GateTriggerBoxArray)
	{
		UBoxComponent* GateTriggerBox = It;
		if (GateTriggerBox)
		{
			GateTriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}

	CloseAllGates();

	SpawnItemBoxes();
}

void AABStageGimmick::OnSetState_Next()
{
	// Stage Trigger Box가 캐릭터를 감지하지 못하도록 설정
	StageTriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
	// Gate Trigger Box는 캐릭터를 감지 가능하도록 설정
	for (const auto& It : GateTriggerBoxArray)
	{
		UBoxComponent* GateTriggerBox = It;
		if (GateTriggerBox)
		{
			GateTriggerBox->SetCollisionProfileName(CPROFILE_ABTRIGGER); // TEXT("ABTrigger)
		}
	}

	OpenAllGates();
}

void AABStageGimmick::SpawnEnemy()
{
	// 지정된 클래스로 Enemy 소환
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.f);
	// SpawnActorDeferred() -> SpawnActor()와 다르게 Actor를 즉시 Spawn하지 않고
	// 필요한 추가 설정들을 모두 완료한 후에 Actor의 FinishSpawning() 함수를
	// 수동으로 호출하여 최종적으로 Spawn을 완료하는 함수
	// Actor가 Spawn되기전에 반드시 필요한 설정값들이 있을 경우 사용하면 좋다
	AABCharacterNonPlayer* EnemyActor = GetWorld()->SpawnActorDeferred<AABCharacterNonPlayer>(
		EnemyClass, SpawnTransform);

	if (EnemyActor)
	{
		// UE가 제공하는 OnDestroyed Delegate에 OnEnemyDestroyed() 함수 bind
		EnemyActor->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnEnemyDestroyed);
		// 현재 스테이지에 맞게 레벨 설정 (점점 증가)
		EnemyActor->SetCharacterLevel(CurrentStageNum);
		// 필요한 설정 모두 끝난 후 완전히 Spawn
		EnemyActor->FinishSpawning(SpawnTransform);
	}
}

void AABStageGimmick::OnEnemyDestroyed(AActor* DestroyedActor)
{
	// 적이 죽으면 STATE_REWARD로 이동
	SetState(EStageState::STATE_REWARD);
}

void AABStageGimmick::SpawnItemBoxes()
{
	for (const auto& It : ItemBoxLocationMap)
	{
		FTransform SpawnTransform(GetActorLocation() + It.Value + FVector(0.f, 0.f, 30.f));
		AABItemBox* ItemBoxActor = GetWorld()->SpawnActorDeferred<AABItemBox>(ItemBoxClass, SpawnTransform);
		if (ItemBoxActor)
		{
			ItemBoxActor->Tags.Add(It.Key);
			ItemBoxActor->GetTriggerBox()->OnComponentBeginOverlap.AddDynamic(
				this, &AABStageGimmick::OnItemBoxTriggerBeginOverlap);
			ItemBoxes.Add(ItemBoxActor);
		}
	}

	// ItemBox는 모든 값들이 반드시 설정된 후 Spawn되어야 함
	for (const auto& ItemBoxActor : ItemBoxes)
	{
		if (ItemBoxActor.IsValid())
		{
			ItemBoxActor->FinishSpawning(ItemBoxActor.Get()->GetActorTransform());
		}
	}
}

void AABStageGimmick::OnItemBoxTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepHitResult)
{
	for (const auto& ItemBoxWeakPtr : ItemBoxes)
	{
		if (ItemBoxWeakPtr.IsValid())
		{
			AABItemBox* ItemBox = ItemBoxWeakPtr.Get();
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			// 유저가 선택한 아이템 박스를 제외하고 모두 파괴하기
			if (ItemBox != OverlappedBox)
			{
				ItemBox->Destroy();
			}
		}
	}

	// 보상 획득 후 STATE_NEXT로 이동
	SetState(EStageState::STATE_NEXT);
}
