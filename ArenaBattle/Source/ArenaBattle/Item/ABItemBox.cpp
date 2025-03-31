#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "Item/ABItemDataBase.h"

AABItemBox::AABItemBox()
{
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));

    //////////////////////////////////////////////////////////////////////////////////////////

    RootComponent = TriggerBox;
    Mesh->SetupAttachment(TriggerBox);
    ParticleEffect->SetupAttachment(TriggerBox);

    TriggerBox->SetCollisionProfileName(CPROFILE_ABTRIGGER); // TEXT("ABTrigger")
    TriggerBox->SetBoxExtent(FVector(40.f, 42.f, 30.f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(
        TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
    if (MeshRef.Object)
    {
        Mesh->SetStaticMesh(MeshRef.Object);
    }

    Mesh->SetRelativeLocation(FVector(0.f, 3.5f, -30.f));
    Mesh->SetCollisionProfileName(TEXT("NoCollision"));

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(
        TEXT("/Script/Engine.ParticleSystem'/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
    if (ParticleRef.Object)
    {
        ParticleEffect->SetTemplate(ParticleRef.Object);
        ParticleEffect->bAutoActivate = false;
    }
}

void AABItemBox::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    UAssetManager& AssetManager = UAssetManager::Get();

    TArray<FPrimaryAssetId> AssetIdList;
    AssetManager.GetPrimaryAssetIdList(TEXT("ABItemData"), AssetIdList);
    ensure(AssetIdList.Num() > 0);
    int32 RandomIndex = FMath::RandRange(0, AssetIdList.Num() - 1);
    FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(AssetIdList[RandomIndex]));
    // SoftObjectPtr�̹Ƿ� ���� �ε��� ���� �ʾҴٸ� �ε������
    if (AssetPtr.IsPending())
    {
        AssetPtr.LoadSynchronous();
    }
    ItemData = Cast<UABItemDataBase>(AssetPtr.Get());
    ensure(ItemData != nullptr);

    // TriggerBox�� �ٸ� Component�� Overlap�� �� ȣ��� Delegate ����
    // (Spawn�� �Ϸ�� ���Ŀ� Delegate�� �����ϵ��� �����ڰ� �ƴ� PostInitializeComponents���� ����)
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
}

void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepHitResult)
{
    if (ItemData == nullptr)
    {
        this->Destroy();
        return;
    }

    // ��ģ ���Ͱ� IABCharacterItemInterface�� ������ ���Ͷ�� �ݹ� �Լ� ȣ��
    IABCharacterItemInterface* OverlappingActor = Cast<IABCharacterItemInterface>(OtherActor);
    if (OverlappingActor != nullptr)
    {
        OverlappingActor->TakeItem(ItemData);
    }

    ParticleEffect->Activate(true);
    // ParticleEffect�� ����� ������ �� ȣ��� Delegate ����
    ParticleEffect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);

    Mesh->SetHiddenInGame(true);
    this->SetActorEnableCollision(false);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
    this->Destroy();
}
