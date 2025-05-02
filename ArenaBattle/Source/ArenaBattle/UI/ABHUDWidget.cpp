#include "UI/ABHUDWidget.h"
#include "UI/ABHpBarWidget.h"
#include "UI/ABCharacterStatWidget.h"
#include "Interface/ABCharacterHUDInterface.h"

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBarWidget = Cast<UABHpBarWidget>(GetWidgetFromName(TEXT("WBP_HpBar")));
	ensure(HpBarWidget);

	CharacterStatWidget = Cast<UABCharacterStatWidget>(GetWidgetFromName(TEXT("WBP_CharacterStat")));
	ensure(CharacterStatWidget);

	IABCharacterHUDInterface* HUDInterface = Cast<IABCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDInterface)
	{
		HUDInterface->SetupHUDWidget(this);
	}
}

void UABHUDWidget::UpdateCharacterStatWidget(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	CharacterStatWidget->UpdateStat(BaseStat, ModifierStat);
}

void UABHUDWidget::UpdateHpBarWidget(float NewCurrentHp, float NewMaxHp)
{
	HpBarWidget->UpdateHpBar(NewCurrentHp, NewMaxHp);
}
