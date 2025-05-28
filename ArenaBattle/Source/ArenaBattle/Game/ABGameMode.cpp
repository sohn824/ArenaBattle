#include "Game/ABGameMode.h"
#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// GameMode에서 사용할 PlayerController 클래스 설정
	PlayerControllerClass = AABPlayerController::StaticClass();

	// 캐릭터 클래스 가져오기
	// (블루프린트 클래스를 가져올 경우 레퍼런스 복사 -> 'Script/Engine.Blueprint' 부분 제거 -> 뒤에 _C 붙이기)
	// "/Script/Engine.Blueprint'/Game/ArenaBattle/Blueprint/BP_ABCharacterPlayer.BP_ABCharacterPlayer'"
	static ConstructorHelpers::FClassFinder<APawn> PawnClassRef(
		TEXT("/Game/ArenaBattle/Blueprint/BP_ABCharacterPlayer.BP_ABCharacterPlayer_C"));
	if (PawnClassRef.Class)
	{
		// GameMode에서 사용할 DefaultPawn 클래스 설정
		DefaultPawnClass = PawnClassRef.Class;
	}

	NeedScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AABGameMode::OnPlayerScoreChanged(int32 NewScore)
{
	CurrentScore = NewScore;

	// 싱글 플레이기 때문에 첫번째 플레이어만 고려
	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ABPlayerController == nullptr)
	{
		return;
	}

	ABPlayerController->OnPlayerScoreChanged(NewScore);

	if (CurrentScore >= NeedScore)
	{
		bIsCleared = true;
		ABPlayerController->OnGameCleared();
	}
	else
	{
		bIsCleared = false;
	}
}

void AABGameMode::OnPlayerDead()
{
	// 싱글 플레이기 때문에 첫번째 플레이어만 고려
	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ABPlayerController == nullptr)
	{
		return;
	}

	ABPlayerController->OnGameOvered();
}

bool AABGameMode::IsGameCleared()
{
	return bIsCleared;
}
