#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTable/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

// �ݹ� ����� ���� ��������Ʈ �����
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float/*NewHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FABCharacterStat&/*BaseStat*/, const FABCharacterStat&/*ModifierStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UABCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZeroCallback;
	FOnHpChangedDelegate OnHpChangedCallback;
	FOnStatChangedDelegate OnStatChangedCallback;

	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	void SetCurrentHp(float NewHp);

	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	void SetCurrentLevelWithStat(int32 NewLevel);

	// ���� ������ �� ������ ��ü�� ������
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	// CharacterStat
	FORCEINLINE const FABCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FABCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FABCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	void SetBaseStat(FABCharacterStat& NewBaseStat);
	void SetModifierStat(FABCharacterStat& NewModifierStat);

	// Damage 
	float ApplyDamage(float& InDamage);

protected:
	// VisibleInstanceOnly - ������ ��ġ�� ������Ʈ���� �ٸ� ���� �������� �� ����
	// Transient - ���� ���� ���̺� �����Ϳ� ������� �ʵ��� �� �� ���� (���ʿ��� �޸� ���� ����)
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float AttackRadius;

	// ĳ���� ��ü �⺻ ���� ������
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	// ���������� �߰��Ǵ� ���� ������
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;
};
