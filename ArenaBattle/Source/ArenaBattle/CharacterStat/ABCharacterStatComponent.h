// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

// 콜백 등록을 위한 델리게이트 선언부
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

	// VisibleInstanceOnly - 레벨에 배치된 오브젝트마다 다른 값을 가져야할 때 지정
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;
	// Transient - 변수 값이 세이브 데이터에 저장되지 않도록 할 때 지정 (불필요한 메모리 낭비 방지)
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
};
