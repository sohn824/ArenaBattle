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
	bUseControllerRotationRoll = false;		// X�� ���� ȸ��
	bUseControllerRotationPitch = false;	// Y�� ���� ȸ��
	bUseControllerRotationYaw = false;		// Z�� ���� ȸ��

	// CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f); // Radius, HalfHeight
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// CharacterMovementComponent
	GetCharacterMovement()->bOrientRotationToMovement = true; // RotationRate���� �����̴� �ӵ��� ������ �ֵ���
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // Pitch, Yaw, Roll
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;	// ü�� ���϶� MaxWalkSpeed ����� (0 ~ 1)
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // �ȴ� ���� �� �ӵ� ���Ұ�

	// SkeletalMeshComponent
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	// SkeletalMeshComponent�� SkeletalMesh�� AnimInstance ����
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

	// �޺� Ÿ�̸Ӱ� ���� ���� �� �Է��� ���Դٸ� ���� ó��
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
	// �ִϸ��̼��� �����ϴ� UAnimInstance Ŭ������ SkeletalMesh�� �پ�����
	const float AttackSpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// ��Ÿ�� �ִϸ��̼��� ���� �� ȣ���� �ݹ� �Լ� ����
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEndCallback);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	// �޺� Ÿ�̸� ����
	ResetComboCheckTimer();
}

void AABCharacterBase::ComboActionEndCallback(class UAnimMontage* TargetMontage, bool bIsProperlyEnded)
{
	ensure(CurrentComboCount > 0);
	
	CurrentComboCount = 0;
	
	// �ٽ� ����Ű �Է� ���� �� �ֵ��� MovementMode ����
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::ResetComboCheckTimer()
{
	int32 ArrayIndex = CurrentComboCount - 1;
	ensure(ComboActionData->EffectiveFrameIndexes.IsValidIndex(ArrayIndex));

	const float AttackSpeedRate = 1.f;
	const int ComboEffectiveFrameIndex = ComboActionData->EffectiveFrameIndexes[ArrayIndex];
	// �޺� �Է��� ���� �� �ִ� �ð�
	float ComboEffectiveTime = ComboEffectiveFrameIndex / ComboActionData->AnimationFrameRate / AttackSpeedRate;
	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboTimerProc, AttackSpeedRate, false);
	}
}

void AABCharacterBase::ComboTimerProc()
{
	ComboTimerHandle.Invalidate();

	// �޺� �Է¿� �����ߴٸ� �ִϸ��̼� ��Ÿ�ָ� ���� �������� �ѱ��
	if (bIsNextComboCommandInputed)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, ComboActionData->MaxComboCount);

		FName NextSectionName = *FString::Printf(TEXT("%s%d")
			, *ComboActionData->MontageSectionNamePrefix, CurrentComboCount);

		AnimInstance->Montage_JumpToSection(NextSectionName, ComboActionMontage);

		// �޺� Ÿ�̸� �����
		ResetComboCheckTimer();

		bIsNextComboCommandInputed = false;
	}
}
