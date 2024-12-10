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

	// UE의 Actor들은 반드시 Root가 될 Component를 지정해줘야 함
	// BodyMeshComponent를 Root Component로 지정
	RootComponent = BodyMeshComponent;

	// WaterMeshComponent를 Root Component의 자식으로 지정
	WaterMeshComponent->SetupAttachment(BodyMeshComponent);
	WaterMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 132.f));

	// StaticMeshComponent의 StaticMesh 지정
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

