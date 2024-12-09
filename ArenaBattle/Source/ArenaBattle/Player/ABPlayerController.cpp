// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 후 마우스 포커스를 바로 뷰포트로 옮기기 위한 코드
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
