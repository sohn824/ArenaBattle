#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTable/ABCharacterStat.h"
#include "ABHUDWidget.generated.h"

/*
 HUD(Head-Up Display) 위젯을 관리하기 위한 기반 클래스
*/

UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UABHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateCharacterStatWidget(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	void UpdateHpBarWidget(float NewHp);

protected:
	UPROPERTY()
	TObjectPtr<class UABHpBarWidget> HpBarWidget;
	UPROPERTY()
	TObjectPtr<class UABCharacterStatWidget> CharacterStatWidget;
};
