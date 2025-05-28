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

void AABPlayerController::OnPlayerScoreChanged(int32 NewScore)
{
	// 블루프린트에서 사용할 콜백 함수도 같이 호출해줌
	K2_OnPlayerScoreChanged(NewScore);
}

void AABPlayerController::OnGameCleared()
{
	// 블루프린트에서 사용할 콜백 함수도 같이 호출해줌
	K2_OnGameCleared();
}

void AABPlayerController::OnGameOvered()
{
	// 블루프린트에서 사용할 콜백 함수도 같이 호출해줌
	K2_OnGameOvered();
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 후 마우스 포커스를 바로 뷰포트로 옮기기 위한 코드
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	// 아래 기능은 블루프린트로 이관
	// 지정해놓은 위젯 블루프린트 클래스로 위젯 생성
	/*ABHUDWidgetPtr = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
	if (ABHUDWidgetPtr)
	{
		ABHUDWidgetPtr->AddToViewport();
	}*/
}
