// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AABCharacterBase::AABCharacterBase()
{
	// Pawn Controller Rotation
	bUseControllerRotationRoll = false;		// X축 기준 회전
	bUseControllerRotationPitch = false;	// Y축 기준 회전
	bUseControllerRotationYaw = false;		// Z축 기준 회전

	// CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f); // Radius, HalfHeight
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// CharacterMovementComponent
	GetCharacterMovement()->bOrientRotationToMovement = true; // RotationRate값이 움직이는 속도에 영향을 주도록
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // Pitch, Yaw, Roll
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;	// 체공 중일때 MaxWalkSpeed 적용률 (0 ~ 1)
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // 걷다 멈출 때 속도 감소값

	// SkeletalMeshComponent
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	// SkeletalMeshComponent에 SkeletalMesh와 AnimInstance 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(
		TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}
