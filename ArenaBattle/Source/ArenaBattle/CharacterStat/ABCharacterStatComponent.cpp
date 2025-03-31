#include "CharacterStat/ABCharacterStatComponent.h"
#include "Game/ABGameSingleton.h"

UABCharacterStatComponent::UABCharacterStatComponent()
{
	CurrentLevel = 1;
	CurrentHp = 100.f;
}

void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentLevelWithStat(CurrentLevel);
	SetCurrentHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetCurrentLevelWithStat(int32 NewLevel)
{
	const int32 MaxLevel = UABGameSingleton::GetInstance().CharacterMaxLevel;
	CurrentLevel = FMath::Clamp<float>(NewLevel, 1, MaxLevel);
	BaseStat = UABGameSingleton::GetInstance().GetCharacterStat(CurrentLevel);
	check(BaseStat.MaxHp > 0.f);
}

void UABCharacterStatComponent::SetCurrentHp(float NewHp)
{
	const int32 MaxHp = UABGameSingleton::GetInstance().GetCharacterStat(CurrentLevel).MaxHp;
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

float UABCharacterStatComponent::ApplyDamage(float& InDamage)
{
	const float PrevHp = CurrentHp;
	InDamage = FMath::Clamp<float>(InDamage, 0.f, InDamage);
	SetCurrentHp(PrevHp - InDamage);

	return InDamage;
}
