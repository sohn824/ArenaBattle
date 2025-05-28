#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * ���� ��� - ���ӿ� �����ϰ� �����ϴ� ���ǰ� ���� �𸮾� ������Ʈ
 * (��Ƽ �÷��̾� ������ ��쿡�� ���ϼ� ����)
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
	uint8 bIsCleared : 1; // bool���� ���� 1��Ʈ�� ���
};
