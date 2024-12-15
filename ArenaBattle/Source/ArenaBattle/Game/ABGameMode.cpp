// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ABGameMode.h"
#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// GameMode에서 사용할 PlayerController 클래스 설정
	PlayerControllerClass = AABPlayerController::StaticClass();

	// 디폴트 3인칭 캐릭터 클래스 가져오기
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(
		TEXT("/Script/ArenaBattle.ABCharacterPlayer"));
	if (ThirdPersonClassRef.Class)
	{
		// GameMode에서 사용할 DefaultPawn 클래스 설정
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}
}

AABGameMode::~AABGameMode()
{

}