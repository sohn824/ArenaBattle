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

	// 게임 시작 후 마우스 포커스를 바로 뷰포트로 옮기기 위한 코드
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	// 지정해놓은 위젯 블루프린트 클래스로 위젯 생성
	ABHUDWidgetPtr = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
	if (ABHUDWidgetPtr)
	{
		ABHUDWidgetPtr->AddToViewport();
	}
}
