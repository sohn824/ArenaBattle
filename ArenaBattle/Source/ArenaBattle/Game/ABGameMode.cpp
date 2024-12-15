// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ABGameMode.h"
#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// GameMode���� ����� PlayerController Ŭ���� ����
	PlayerControllerClass = AABPlayerController::StaticClass();

	// ����Ʈ 3��Ī ĳ���� Ŭ���� ��������
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(
		TEXT("/Script/ArenaBattle.ABCharacterPlayer"));
	if (ThirdPersonClassRef.Class)
	{
		// GameMode���� ����� DefaultPawn Ŭ���� ����
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}
}

AABGameMode::~AABGameMode()
{

}