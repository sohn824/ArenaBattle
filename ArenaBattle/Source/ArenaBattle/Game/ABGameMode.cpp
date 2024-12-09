// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ABGameMode.h"
#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// GameMode에서 사용할 PlayerController 클래스 설정
	PlayerControllerClass = AABPlayerController::StaticClass();

	// 디폴트 3인칭 캐릭터 클래스 가져오기
	// 애셋 우클릭 후 레퍼런스 복사로 경로를 가져온 후 '_C'를 붙여주면 레퍼런스를 가져올 수 있음
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	if (ThirdPersonClassRef.Class)
	{
		// GameMode에서 사용할 DefaultPawn 클래스 설정
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}
}

AABGameMode::~AABGameMode()
{

}