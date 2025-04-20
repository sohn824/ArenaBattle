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
	// SpringArm (카메라 지지대)
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

	// 시점 관련 데이터 초기화 및 InputMappingContext 추가
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

	// EnhancedInputComponent를 사용하지 않은 경우 에러
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// InputComponent에 액션별 함수 매핑
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
			// InputMappingContext 교체
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
			// InputMappingContext 교체
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(QuaterViewControlData->InputMappingContext, 0);

			CurrentCharacterControlType = ECharacterControlType::QuaterView;
		}
	}
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// Controller의 회전값 중 Yaw값을 참고하여 이동 방향 설정 
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& InputActionValue)
{
	// 마우스 입력값으로 Controller의 회전값 설정
	// ~키 입력으로 콘솔창에서 'DisplayAll PlayerController ControlRotation'
	// 명령어를 통해 현재 Pitch, Yaw, Roll 입력값을 확인할 수 있음
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AABCharacterPlayer::QuaterMove(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	// 이동 입력값 벡터 크기가 1보다 크다면 정규화시킴
	if (MovementVectorSizeSquared > 1.f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}

	float MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	// 캐릭터가 이동할 방향으로 회전시키기
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

	// HUD Widget에 스탯이 변동됐음을 알려줄 Delegate 등록
	StatComponent->OnStatChangedCallback.AddUObject(InHUDWidget, &UABHUDWidget::UpdateCharacterStatWidget);
	// HUD Widget에 HP가 변동됐음을 알려줄 Delegate 등록
	StatComponent->OnHpChangedCallback.AddUObject(InHUDWidget, &UABHUDWidget::UpdateHpBarWidget);
}