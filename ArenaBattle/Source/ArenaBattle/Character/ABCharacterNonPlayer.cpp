// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterNonPlayer.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{

}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// NonPlayer 캐릭터는 죽은 후 일정 시간이 지나면 사라지도록 함
	const float DestroyDelayTime = 5.f;
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
		FTimerDelegate::CreateLambda(
			[&]()
			{
				Destroy();
			}), DestroyDelayTime, false);
}
