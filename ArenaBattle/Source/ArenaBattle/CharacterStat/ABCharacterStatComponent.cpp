#include "CharacterStat/ABCharacterStatComponent.h"
#include "Game/ABGameSingleton.h"

UABCharacterStatComponent::UABCharacterStatComponent()
{
	CurrentHp = 100.f;
	CurrentLevel = 1;
	AttackRadius = 50.f;

	// InitializeComponent 콜백을 호출시키기 위해 필요
	// (호출 시점은 BeginPlay가 호출되기 이전)
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

	// Hp 변경 콜백 호출
	OnHpChangedCallback.Broadcast(CurrentHp);
	// Hp값이 부동 소수점이므로 Dead 체크를 위해 0.f대신 안전한 작은 값 사용
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// Hp 다 깎였을 때 콜백호출
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
