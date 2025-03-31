// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ABCharacterBase.h"
#include "Character/ABCharacterControlData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "ABComboActionData.h"
#include "Physics/ABCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "UI/ABWidgetComponent.h"
#include "UI/ABHpBarWidget.h"
#include "Item/ABWeaponItemData.h"

// Sets default values
AABCharacterBase::AABCharacterBase()
{
	// Pawn Controller Rotation
	bUseControllerRotationRoll = false;		// X�� ���� ȸ��
	bUseControllerRotationPitch = false;	// Y�� ���� ȸ��
	bUseControllerRotationYaw = false;		// Z�� ���� ȸ��

	// CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f); // Radius, HalfHeight
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE); // TEXT("ABCapsule")

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
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision")); // CapsuleComponent�� �浹 ������ �����ϹǷ� SkeletalMesh�� �浹 ���� ���ϵ���

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

	// Character Control Data
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

	// Combo Action AnimMontage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(
		TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	// Dead AnimMontage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(
		TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// Combo Action Data (�޺� �׼ǿ� �ʿ��� ������ ���� - UPrimaryDataAsset ����)
	static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(
		TEXT("/Script/ArenaBattle.ABComboActionData'/Game/ArenaBattle/CharacterActionData/ABA_ComboAttack.ABA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	// Character Stat Component
	StatComponent = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("StatComponent"));

	// Hp Bar UI Widget Component
	HpBarComponent = CreateDefaultSubobject<UABWidgetComponent>(TEXT("HpBarComponent"));
	// ĳ������ ���̷�Ż �޽ø� �θ�� ����
	HpBarComponent->SetupAttachment(GetMesh());
	HpBarComponent->SetRelativeLocation(FVector(0.f, 0.f, 230.f));
	// Ŭ���� ���� ����
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetClassRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetClassRef.Class)
	{
		HpBarComponent->SetWidgetClass(HpBarWidgetClassRef.Class);
		// ���� ��ǥ�� �ƴ� ��ũ�� ��ǥ�� ���� (2D)
		HpBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarComponent->SetDrawSize(FVector2D(150.f, 15.f));
		HpBarComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Item Function Delegates
	TakeItemDelegates.Add(FOnTakeItemDelegateWrapper(
		FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::EquipWeapon)));
	TakeItemDelegates.Add(FOnTakeItemDelegateWrapper(
		FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::DrinkPotion)));
	TakeItemDelegates.Add(FOnTakeItemDelegateWrapper(
		FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::ReadScroll)));

	// Weapon
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	// ���� - Skeletal Mesh�� Ư�� ��ġ pivot�� �̸��� �ٿ� ������ �� �ִ� ���
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AABCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Hp�� 0�� ���� �� ȣ��� �ݹ� ��������Ʈ�� Dead �ִϸ��̼� ��� �Լ��� ����صд�.
	StatComponent->OnHpZeroCallback.AddUObject(this, &AABCharacterBase::SetDead);
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
	const float AttackSpeedRate = StatComponent->GetTotalStat().AttackSpeed;
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

	const float AttackSpeedRate = StatComponent->GetTotalStat().AttackSpeed;
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

// ���� ������ �ϴ� Animation Notify���� ȣ��� �ݹ� �Լ�
void AABCharacterBase::AttackHitCheck()
{
	// �浹 ������ �ش� ���������� ���� ������ �����ϴ� ����ü
	// SweepSingleByChannel()�� ���� �Լ��� �����ϸ� �浹 ������ �ϰ� ���� ������ ��ȯ��
	FHitResult OutHitResult;

	// �浹 �Լ��� ���޵Ǵ� �Ű����� ����ü
	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(Attack), false, this);

	const float& AttackRange = StatComponent->GetTotalStat().AttackRange;
	const float& AttackDamage = StatComponent->GetTotalStat().AttackDamage;
	const FVector StartPos = GetActorLocation()
		+ GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector EndPos = StartPos + GetActorForwardVector() * AttackRange;

	// Ư�� ä���� ����Ͽ� �浹 �˻縦 ����
	// StartPos���� EndPos���� ������ �浹ü�� �̵���Ű�鼭 �浹�� �����ϰ�
	// FHitResult ����ü�� �浹 ���� �������� ��� ��ȯ��
	const float AttackRadius = 50.f;
	bool bHitSucceed = GetWorld()->SweepSingleByChannel(OutHitResult, StartPos, EndPos,
		FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), CollisionParams);
	if (bHitSucceed)
	{
		FDamageEvent DamageEvent;
		// ���� ������ TakeDamage �Լ� ȣ��
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#ifdef ENABLE_DRAW_DEBUG
	// �浹 ������
	FVector CapsuleOrigin = StartPos + (EndPos - StartPos) * 0.5f;
	const float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = bHitSucceed ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
#endif //ENABLE_DRAW_DEBUG
}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// EventInstigator - �������� ���� �÷��̾� (��Ʈ�ѷ�)
	// DamageCauser - �������� ���� ���� (�÷��̾ ������ ���̳� �װ��� �߻��� �߻�ü ���)
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float FinalDamageAmount = DamageAmount;
	StatComponent->ApplyDamage(FinalDamageAmount);
	// �������� ���� ������ ��ȯ
	return FinalDamageAmount;
}

void AABCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);
	PlayDeadAnimation();
	HpBarComponent->SetHiddenInGame(true);
}

void AABCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(DeadMontage, 1.f);
}

int32 AABCharacterBase::GetCharacterLevel()
{
	return StatComponent->GetCurrentLevel();
}

void AABCharacterBase::SetCharacterLevel(int32 NewLevel)
{
	StatComponent->SetCurrentLevelWithStat(NewLevel);
}

void AABCharacterBase::SetupCharacterWidget(UABUserWidget* InUserWidget)
{
	UABHpBarWidget* HpBarWidget = Cast<UABHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		const float& MaxHp = StatComponent->GetTotalStat().MaxHp;
		const float& CurrentHp = StatComponent->GetCurrentHp();
		HpBarWidget->SetMaxHp(MaxHp);
		HpBarWidget->UpdateHpBar(CurrentHp);

		// HpChanged �ݹ��� ȣ��� ������ HpBarWidget�� ���� ������Ʈ�ϵ���
		// UABHpBarWidget::UpdateHpBar �Լ��� ��������Ʈ�� ���
		StatComponent->OnHpChangedCallback.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
	}
}

void AABCharacterBase::TakeItem(UABItemDataBase* InItemData)
{
	if (InItemData == nullptr)
	{
		return;
	}

	// ������ Ÿ�Կ� �´� �Լ� ����
	uint8 ArrayIdx = (uint8)InItemData->ItemType;
	if (TakeItemDelegates.IsValidIndex(ArrayIdx) == false)
	{
		return;
	}

	TakeItemDelegates[ArrayIdx].Delegate.ExecuteIfBound(InItemData);
}

void AABCharacterBase::EquipWeapon(UABItemDataBase* InItemData)
{
	UABWeaponItemData* WeaponItemData = Cast<UABWeaponItemData>(InItemData);
	if (WeaponItemData == nullptr)
	{
		return;
	}

	// WeaponItemData�� WeaponMesh�� �޸� ������ ����
	// TSoftObjectPtr�� ����Ʈ ���۷��� �Ǿ� �����Ƿ�
	// �޸𸮿� ���� �� �ö� �ִٸ� �ε�������
	if (WeaponItemData->WeaponMesh.IsPending())
	{
		WeaponItemData->WeaponMesh.LoadSynchronous();
	}

	WeaponMesh->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());

	StatComponent->SetModifierStat(WeaponItemData->ModifierStat);
}

void AABCharacterBase::DrinkPotion(UABItemDataBase* InItemData)
{
	UE_LOG(LogTemp, Log, TEXT("Drink Potion"));
}

void AABCharacterBase::ReadScroll(UABItemDataBase* InItemData)
{
	UE_LOG(LogTemp, Log, TEXT("Read Scroll"));
}
