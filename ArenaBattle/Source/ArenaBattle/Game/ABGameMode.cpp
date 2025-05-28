#include "Game/ABGameMode.h"
#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// GameMode���� ����� PlayerController Ŭ���� ����
	PlayerControllerClass = AABPlayerController::StaticClass();

	// ĳ���� Ŭ���� ��������
	// (�������Ʈ Ŭ������ ������ ��� ���۷��� ���� -> 'Script/Engine.Blueprint' �κ� ���� -> �ڿ� _C ���̱�)
	// "/Script/Engine.Blueprint'/Game/ArenaBattle/Blueprint/BP_ABCharacterPlayer.BP_ABCharacterPlayer'"
	static ConstructorHelpers::FClassFinder<APawn> PawnClassRef(
		TEXT("/Game/ArenaBattle/Blueprint/BP_ABCharacterPlayer.BP_ABCharacterPlayer_C"));
	if (PawnClassRef.Class)
	{
		// GameMode���� ����� DefaultPawn Ŭ���� ����
		DefaultPawnClass = PawnClassRef.Class;
	}

	NeedScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AABGameMode::OnPlayerScoreChanged(int32 NewScore)
{
	CurrentScore = NewScore;

	// �̱� �÷��̱� ������ ù��° �÷��̾ ���
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
	// �̱� �÷��̱� ������ ù��° �÷��̾ ���
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
