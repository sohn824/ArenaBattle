// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ABCharacterBase.h"
#include "Character/ABCharacterControlData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "ABComboActionData.h"

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
		TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(
		TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderViewDataRef(
		TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControlData/ABC_ShoulderView.ABC_ShoulderView'"));
	if (ShoulderViewDataRef.Object)
	{
		CharacterControlDataMap.Add(ECharacterControlType::ShoulderView, ShoulderViewDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterViewDataRef(
		TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControlData/ABC_QuaterView.ABC_QuaterView'"));
	if (QuaterViewDataRef.Object)
	{
		CharacterControlDataMap.Add(ECharacterControlType::QuaterView, QuaterViewDataRef.Object);
	}
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AABCharacterBase::ComboActionProcess()
{
	if (CurrentComboCount == 0)
	{
		ComboActionBeginCallback();
		return;
	}

	// 콤보 타이머가 돌고 있을 때 입력이 들어왔다면 성공 처리
	if (ComboTimerHandle.IsValid())
	{
		bIsNextComboCommandInputed = true;
	}
	else
	{
		bIsNextComboCommandInputed = false;
	}
}

void AABCharacterBase::ComboActionBeginCallback()
{
	// Combo Status
	CurrentComboCount = 1;

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	// 애니메이션을 관리하는 UAnimInstance 클래스는 SkeletalMesh에 붙어있음
	const float AttackSpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// 몽타주 애니메이션이 끝날 때 호출할 콜백 함수 지정
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEndCallback);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	// 콤보 타이머 시작
	ResetComboCheckTimer();
}

void AABCharacterBase::ComboActionEndCallback(class UAnimMontage* TargetMontage, bool bIsProperlyEnded)
{
	ensure(CurrentComboCount > 0);
	
	CurrentComboCount = 0;
	
	// 다시 방향키 입력 받을 수 있도록 MovementMode 복구
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::ResetComboCheckTimer()
{
	int32 ArrayIndex = CurrentComboCount - 1;
	ensure(ComboActionData->EffectiveFrameIndexes.IsValidIndex(ArrayIndex));

	const float AttackSpeedRate = 1.f;
	const int ComboEffectiveFrameIndex = ComboActionData->EffectiveFrameIndexes[ArrayIndex];
	// 콤보 입력을 넣을 수 있는 시간
	float ComboEffectiveTime = ComboEffectiveFrameIndex / ComboActionData->AnimationFrameRate / AttackSpeedRate;
	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboTimerProc, AttackSpeedRate, false);
	}
}

void AABCharacterBase::ComboTimerProc()
{
	ComboTimerHandle.Invalidate();

	// 콤보 입력에 성공했다면 애니메이션 몽타주를 다음 섹션으로 넘기기
	if (bIsNextComboCommandInputed)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, ComboActionData->MaxComboCount);

		FName NextSectionName = *FString::Printf(TEXT("%s%d")
			, *ComboActionData->MontageSectionNamePrefix, CurrentComboCount);

		AnimInstance->Montage_JumpToSection(NextSectionName, ComboActionMontage);

		// 콤보 타이머 재시작
		ResetComboCheckTimer();

		bIsNextComboCommandInputed = false;
	}
}
