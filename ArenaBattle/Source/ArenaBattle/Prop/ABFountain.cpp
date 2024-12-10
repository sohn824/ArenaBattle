// Fill out your copyright notice in the Description page of Project Settings.

#include "Prop/ABFountain.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AABFountain::AABFountain()
{
 	// Set this actor to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	WaterMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMesh"));

	// UE�� Actor���� �ݵ�� Root�� �� Component�� ��������� ��
	// BodyMeshComponent�� Root Component�� ����
	RootComponent = BodyMeshComponent;

	// WaterMeshComponent�� Root Component�� �ڽ����� ����
	WaterMeshComponent->SetupAttachment(BodyMeshComponent);
	WaterMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 132.f));

	// StaticMeshComponent�� StaticMesh ����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));
	if (BodyMeshRef.Object)
	{
		BodyMeshComponent->SetStaticMesh(BodyMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));
	if (WaterMeshRef.Object)
	{
		WaterMeshComponent->SetStaticMesh(WaterMeshRef.Object);
	}
}

// Called when the game starts or when spawned
void AABFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

