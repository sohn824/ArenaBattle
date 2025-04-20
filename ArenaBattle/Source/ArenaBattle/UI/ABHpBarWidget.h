// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// UserWidget의 사용자 재정의 클래스
#include "ABUserWidget.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	// UMG 위젯이 생성될 때 호출되는 콜백 함수 (BP의 Event Construct)
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;
	UPROPERTY()
	float MaxHp;
};
