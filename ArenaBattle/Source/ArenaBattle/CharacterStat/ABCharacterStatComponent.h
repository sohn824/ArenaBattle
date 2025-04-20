#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTable/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

// 콜백 등록을 위한 델리게이트 선언부
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

	// 공격 판정을 할 가상의 구체의 반지름
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
	// VisibleInstanceOnly - 레벨에 배치된 오브젝트마다 다른 값을 가져야할 때 지정
	// Transient - 변수 값이 세이브 데이터에 저장되지 않도록 할 때 지정 (불필요한 메모리 낭비 방지)
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float AttackRadius;

	// 캐릭터 자체 기본 스탯 데이터
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	// 아이템으로 추가되는 스탯 데이터
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;
};
