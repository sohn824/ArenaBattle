// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ���콺 ��Ŀ���� �ٷ� ����Ʈ�� �ű�� ���� �ڵ�
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
