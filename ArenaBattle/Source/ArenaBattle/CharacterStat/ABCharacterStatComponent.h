// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float& InDamage);

protected:
	void SetCurrentHp(float NewHp);

	// VisibleInstanceOnly - ������ ��ġ�� ������Ʈ���� �ٸ� ���� �������� �� ����
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;
	// Transient - ���� ���� ���̺� �����Ϳ� ������� �ʵ��� �� �� ���� (���ʿ��� �޸� ���� ����)
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
};
