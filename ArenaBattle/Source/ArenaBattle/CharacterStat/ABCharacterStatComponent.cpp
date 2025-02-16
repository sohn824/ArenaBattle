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

	// Hp ���� �ݹ� ȣ��
	OnHpChangedCallback.Broadcast(CurrentHp);
	// Hp���� �ε� �Ҽ����̹Ƿ� Dead üũ�� ���� 0.f��� ������ ���� �� ���
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// Hp �� ���� �� �ݹ�ȣ��
		OnHpZeroCallback.Broadcast();
	}
}
