#include "Character/ABCharacterPlayer.h"
#include "Character/ABCharacterControlData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/ABHUDWidget.h"
#include "CharacterStat/ABCharacterStatComponent.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	// SpringArm (ī�޶� ������)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeViewActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangeView.IA_ChangeView'"));
	if (ChangeViewActionRef.Object)
	{
		ChangeViewAction = ChangeViewActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderMoveActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (ShoulderMoveActionRef.Object)
	{
		ShoulderMoveAction = ShoulderMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderLookActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (ShoulderLookActionRef.Object)
	{
		ShoulderLookAction = ShoulderLookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuaterMoveActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (QuaterMoveActionRef.Object)
	{
		QuaterMoveAction = QuaterMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack'"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::QuaterView;
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer());

	// ���� ���� ������ �ʱ�ȭ �� InputMappingContext �߰�
	if (Subsystem)
	{
		UABCharacterControlData* CurrentCharacterControlData = CharacterControlDataMap[CurrentCharacterControlType];
		if (CurrentCharacterControlData)
		{
			SetCharacterControlData(CurrentCharacterControlData);
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(CurrentCharacterControlData->InputMappingContext, 0);

			CurrentCharacterControlType = ECharacterControlType::QuaterView;
		}
	}
}

void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhancedInputComponent�� ������� ���� ��� ����
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// InputComponent�� �׼Ǻ� �Լ� ����
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeViewAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::OnChangeView);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Attack);
}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AABCharacterPlayer::OnChangeView()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer());

	if (Subsystem == nullptr)
	{
		return;
	}

	if (CurrentCharacterControlType == ECharacterControlType::QuaterView)
	{
		UABCharacterControlData* ShoulderViewControlData = CharacterControlDataMap[ECharacterControlType::ShoulderView];
		if (ShoulderViewControlData)
		{
			SetCharacterControlData(ShoulderViewControlData);
			// InputMappingContext ��ü
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(ShoulderViewControlData->InputMappingContext, 0);

			CurrentCharacterControlType = ECharacterControlType::ShoulderView;
		}
	}
	else if (CurrentCharacterControlType == ECharacterControlType::ShoulderView)
	{
		UABCharacterControlData* QuaterViewControlData = CharacterControlDataMap[ECharacterControlType::QuaterView];
		if (QuaterViewControlData)
		{
			SetCharacterControlData(QuaterViewControlData);
			// InputMappingContext ��ü
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(QuaterViewControlData->InputMappingContext, 0);

			CurrentCharacterControlType = ECharacterControlType::QuaterView;
		}
	}
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// Controller�� ȸ���� �� Yaw���� �����Ͽ� �̵� ���� ���� 
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& InputActionValue)
{
	// ���콺 �Է°����� Controller�� ȸ���� ����
	// ~Ű �Է����� �ܼ�â���� 'DisplayAll PlayerController ControlRotation'
	// ��ɾ ���� ���� Pitch, Yaw, Roll �Է°��� Ȯ���� �� ����
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AABCharacterPlayer::QuaterMove(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	// �̵� �Է°� ���� ũ�Ⱑ 1���� ũ�ٸ� ����ȭ��Ŵ
	if (MovementVectorSizeSquared > 1.f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}

	float MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	// ĳ���Ͱ� �̵��� �������� ȸ����Ű��
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());

	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AABCharacterPlayer::Attack()
{
	ComboActionProcess();
}

void AABCharacterPlayer::SetupHUDWidget(UABHUDWidget* InHUDWidget)
{
	if (InHUDWidget == nullptr)
	{
		return;
	}

	InHUDWidget->UpdateCharacterStatWidget(
		StatComponent->GetBaseStat(), StatComponent->GetModifierStat());
	InHUDWidget->UpdateHpBarWidget(StatComponent->GetCurrentHp());

	// HUD Widget�� ������ ���������� �˷��� Delegate ���
	StatComponent->OnStatChangedCallback.AddUObject(InHUDWidget, &UABHUDWidget::UpdateCharacterStatWidget);
	// HUD Widget�� HP�� ���������� �˷��� Delegate ���
	StatComponent->OnHpChangedCallback.AddUObject(InHUDWidget, &UABHUDWidget::UpdateHpBarWidget);
}