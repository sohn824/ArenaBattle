#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * 게임 모드 - 게임에 유일하게 존재하는 심판과 같은 언리얼 오브젝트
 * (멀티 플레이어 게임일 경우에도 유일성 보장)
 */
UCLASS()
class ARENABATTLE_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AABGameMode();

public:
	void OnPlayerScoreChanged(int32 NewScore);
	void OnPlayerDead();
	bool IsGameCleared();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 NeedScore;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1; // bool값을 위해 1비트만 사용
};
