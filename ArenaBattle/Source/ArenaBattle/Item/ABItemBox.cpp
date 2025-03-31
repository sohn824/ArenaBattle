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
    // SoftObjectPtr이므로 아직 로딩이 되지 않았다면 로드시켜줌
    if (AssetPtr.IsPending())
    {
        AssetPtr.LoadSynchronous();
    }
    ItemData = Cast<UABItemDataBase>(AssetPtr.Get());
    ensure(ItemData != nullptr);

    // TriggerBox에 다른 Component가 Overlap될 때 호출될 Delegate 연결
    // (Spawn이 완료된 이후에 Delegate가 동작하도록 생성자가 아닌 PostInitializeComponents에서 설정)
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

    // 겹친 액터가 IABCharacterItemInterface를 구현한 액터라면 콜백 함수 호출
    IABCharacterItemInterface* OverlappingActor = Cast<IABCharacterItemInterface>(OtherActor);
    if (OverlappingActor != nullptr)
    {
        OverlappingActor->TakeItem(ItemData);
    }

    ParticleEffect->Activate(true);
    // ParticleEffect가 재생이 끝났을 때 호출될 Delegate 연결
    ParticleEffect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);

    Mesh->SetHiddenInGame(true);
    this->SetActorEnableCollision(false);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
    this->Destroy();
}
