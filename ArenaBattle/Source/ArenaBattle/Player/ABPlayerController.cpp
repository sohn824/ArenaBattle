#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"

AABPlayerController::AABPlayerController()
{	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(
		TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ���콺 ��Ŀ���� �ٷ� ����Ʈ�� �ű�� ���� �ڵ�
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	// �����س��� ���� �������Ʈ Ŭ������ ���� ����
	ABHUDWidgetPtr = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
	if (ABHUDWidgetPtr)
	{
		ABHUDWidgetPtr->AddToViewport();
	}
}
