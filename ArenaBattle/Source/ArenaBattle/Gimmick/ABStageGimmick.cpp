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
			// ComponentTags - �׷�ȭ�� ���� FName �±� �迭
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

// Unreal Editor���� �� ������ ���� �� ȣ��Ǵ� ������ �ݹ�
void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepHitResult)
{
	// ĳ���Ͱ� Stage�� ���� TriggerBox�� �����Ǹ� STATE_FIGHT�� �̵�
	SetState(EStageState::STATE_FIGHT);
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepHitResult)
{
	// OverlappedComponent - �浹�� ������ Component (���⼭�� GateMeshComponent)
	// +XGate, -XGate, +YGate, -YGate �� �ϳ��� ComponentTag�� �޾Ƴ���
	if (OverlappedComponent->ComponentTags.Num() <= 0)
	{
		return;
	}

	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// SocketName�� +X, -X, +Y, -Y �� �ϳ�
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	if (StageMeshComponent->DoesSocketExist(SocketName) == false)
	{
		return;
	}

	FVector NewLocation = StageMeshComponent->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;

	// FCollisonQueryParams - UE�� �浹 �˻縦 ���� �ʿ��� �ɼǰ����� ����ü
	// @ TraceTag - ������ �±�
	// @ bInTraceComplex - ������ �浹�˻� ��� ����
	// @ InIgnoreActor - �浹�˻翡�� ������ Actor ������
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(GateTrigger), false, this);

	// ���� �� ����� ���� ���� Stage�� �����ϰ�
	// Ư�� �ݰ� ���� ������Ʈ�� ������ Ȯ�� �� ���ٸ� Stage�� ���� ������
	// (�̹� Stage�� �ִ� ������ ���� �����Ƿ�)
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

	// State�� �°� bind�� Delegate ȣ�����ֱ�
	if (OnSetStateDelegateMap.Contains(NewState))
	{
		OnSetStateDelegateMap[NewState].Delegate.ExecuteIfBound();
	}
}

void AABStageGimmick::OnSetState_Ready()
{
	// Stage Trigger Box�� ĳ���͸� ���� �����ϵ��� ����
	StageTriggerBox->SetCollisionProfileName(CPROFILE_ABTRIGGER); // TEXT("ABTrigger")
	// Gate Trigger Box�� ĳ���͸� �������� ���ϵ��� ����
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

	// EnemySpawnFirstDelay �� SpawnEnemy() ����
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
	// Stage Trigger Box�� ĳ���͸� �������� ���ϵ��� ����
	StageTriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
	// Gate Trigger Box�� ĳ���͸� ���� �����ϵ��� ����
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
	// ������ Ŭ������ Enemy ��ȯ
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.f);
	// SpawnActorDeferred() -> SpawnActor()�� �ٸ��� Actor�� ��� Spawn���� �ʰ�
	// �ʿ��� �߰� �������� ��� �Ϸ��� �Ŀ� Actor�� FinishSpawning() �Լ���
	// �������� ȣ���Ͽ� ���������� Spawn�� �Ϸ��ϴ� �Լ�
	// Actor�� Spawn�Ǳ����� �ݵ�� �ʿ��� ���������� ���� ��� ����ϸ� ����
	AABCharacterNonPlayer* EnemyActor = GetWorld()->SpawnActorDeferred<AABCharacterNonPlayer>(
		EnemyClass, SpawnTransform);

	if (EnemyActor)
	{
		// UE�� �����ϴ� OnDestroyed Delegate�� OnEnemyDestroyed() �Լ� bind
		EnemyActor->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnEnemyDestroyed);
		// ���� ���������� �°� ���� ���� (���� ����)
		EnemyActor->SetCharacterLevel(CurrentStageNum);
		// �ʿ��� ���� ��� ���� �� ������ Spawn
		EnemyActor->FinishSpawning(SpawnTransform);
	}
}

void AABStageGimmick::OnEnemyDestroyed(AActor* DestroyedActor)
{
	// ���� ������ STATE_REWARD�� �̵�
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

	// ItemBox�� ��� ������ �ݵ�� ������ �� Spawn�Ǿ�� ��
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
			// ������ ������ ������ �ڽ��� �����ϰ� ��� �ı��ϱ�
			if (ItemBox != OverlappedBox)
			{
				ItemBox->Destroy();
			}
		}
	}

	// ���� ȹ�� �� STATE_NEXT�� �̵�
	SetState(EStageState::STATE_NEXT);
}
