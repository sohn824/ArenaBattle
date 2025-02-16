// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	MaxHp = 100.f;
	CurrentHp = 100.f;
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
}

float UABCharacterStatComponent::ApplyDamage(float& InDamage)
{
	const float PrevHp = CurrentHp;
	InDamage = FMath::Clamp<float>(InDamage, 0.f, InDamage);
	SetCurrentHp(PrevHp - InDamage);

	return InDamage;
}

void UABCharacterStatComponent::SetCurrentHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, MaxHp);

	// Hp 변경 콜백 호출
	OnHpChangedCallback.Broadcast(CurrentHp);
	// Hp값이 부동 소수점이므로 Dead 체크를 위해 0.f대신 안전한 작은 값 사용
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// Hp 다 깎였을 때 콜백호출
		OnHpZeroCallback.Broadcast();
	}
}
