#pragma once

#include "CoreMinimal.h"
// UserWidget�� ����� ������ Ŭ����
#include "ABUserWidget.h"
#include "DataTable/ABCharacterStat.h"
#include "ABHpBarWidget.generated.h"

/**
 * ĳ���� ���� �پ ����ٴϴ� HpBar ���� Ŭ����
 */
UCLASS()
class ARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	// UMG ������ ������ �� ȣ��Ǵ� �ݹ� �Լ� (BP�� Event Construct)
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
