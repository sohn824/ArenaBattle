#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentHp = -1.f;
	MaxHp = -1.f;
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	ensure(HpProgressBar);
	HpStatTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_HpStat")));
	ensure(HpStatTextBlock);

	IABCharacterWidgetInterface* CharacterWidgetInterface = Cast<IABCharacterWidgetInterface>(OwnerActor);
	// OwnerActor가 IABCharacterWidgetInterface를 가지고 있는지 검사
	if (CharacterWidgetInterface)
	{
		// IABCharacterWidgetInterface를 가지고 있다면 OwnerActor에 HpBarWidget 세팅
		CharacterWidgetInterface->SetupCharacterWidget(this);
	}
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	ensure(NewMaxHp > 0.f);
	ensure(HpProgressBar != nullptr);

	CurrentHp = NewCurrentHp;
	MaxHp = NewMaxHp;

	float HpPercent = NewCurrentHp / NewMaxHp;
	HpProgressBar->SetPercent(HpPercent);
	UpdateHpStatTextBlock();
}

void UABHpBarWidget::UpdateHpStatTextBlock()
{
	FString HpStatString = FString::Printf(TEXT("%.0f / %.0f"), CurrentHp, MaxHp);
	HpStatTextBlock->SetText(FText::FromString(HpStatString));
}
