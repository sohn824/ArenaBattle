// Fill out your copyright notice in the Description page of Project Settings.

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
}

AABGameMode::~AABGameMode()
{

}