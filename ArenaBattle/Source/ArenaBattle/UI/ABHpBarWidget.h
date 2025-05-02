#pragma once

#include "CoreMinimal.h"
// UserWidget의 사용자 재정의 클래스
#include "ABUserWidget.h"
#include "DataTable/ABCharacterStat.h"
#include "ABHpBarWidget.generated.h"

/**
 * 캐릭터 위에 붙어서 따라다니는 HpBar 위젯 클래스
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
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);
	void UpdateHpStatTextBlock();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;
	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStatTextBlock;
	UPROPERTY()
	float CurrentHp;
	UPROPERTY()
	float MaxHp;
};
