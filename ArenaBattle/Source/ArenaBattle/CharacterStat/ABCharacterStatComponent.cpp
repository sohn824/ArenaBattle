#include "CharacterStat/ABCharacterStatComponent.h"
#include "Game/ABGameSingleton.h"

UABCharacterStatComponent::UABCharacterStatComponent()
{
	CurrentHp = 100.f;
	CurrentLevel = 1;
	AttackRadius = 50.f;

	// InitializeComponent �ݹ��� ȣ���Ű�� ���� �ʿ�
	// (ȣ�� ������ BeginPlay�� ȣ��Ǳ� ����)
	bWantsInitializeComponent = true;
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetCurrentLevelWithStat(CurrentLevel);
	SetCurrentHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetCurrentLevelWithStat(int32 NewLevel)
{
	const int32 MaxLevel = UABGameSingleton::GetInstance().CharacterMaxLevel;
	CurrentLevel = FMath::Clamp<float>(NewLevel, 1, MaxLevel);

	FABCharacterStat NewBaseStat = UABGameSingleton::GetInstance().GetCharacterStat(CurrentLevel);
	check(NewBaseStat.MaxHp > 0.f);

	SetBaseStat(NewBaseStat);
}

void UABCharacterStatComponent::SetBaseStat(FABCharacterStat& NewBaseStat)
{
	BaseStat = NewBaseStat;

	OnStatChangedCallback.Broadcast(BaseStat, ModifierStat);
}

void UABCharacterStatComponent::SetModifierStat(FABCharacterStat& NewModifierStat)
{
	ModifierStat = NewModifierStat;

	OnStatChangedCallback.Broadcast(BaseStat, ModifierStat);
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
