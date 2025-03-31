// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTable/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

// �ݹ� ����� ���� ��������Ʈ �����
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZeroCallback;
	FOnHpChangedDelegate OnHpChangedCallback;

	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	void SetCurrentHp(float NewHp);

	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	void SetCurrentLevelWithStat(int32 NewLevel);

	FORCEINLINE FABCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE void SetModifierStat(FABCharacterStat& NewModifierStat) { ModifierStat = NewModifierStat; }

	float ApplyDamage(float& InDamage);

protected:
	// VisibleInstanceOnly - ������ ��ġ�� ������Ʈ���� �ٸ� ���� �������� �� ����
	// Transient - ���� ���� ���̺� �����Ϳ� ������� �ʵ��� �� �� ���� (���ʿ��� �޸� ���� ����)
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentLevel;

	// ĳ���� ��ü �⺻ ���� ������
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	// ���������� �߰��Ǵ� ���� ������
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;
};
