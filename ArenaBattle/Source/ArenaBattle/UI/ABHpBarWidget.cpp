// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxHp = -1.f;
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));

	IABCharacterWidgetInterface* CharacterWidgetInterface = Cast<IABCharacterWidgetInterface>(OwnerActor);
	// OwnerActor가 IABCharacterWidgetInterface를 가지고 있는지 검사
	if (CharacterWidgetInterface)
	{
		// IABCharacterWidgetInterface를 가지고 있다면 OwnerActor에 HpBarWidget 세팅
		CharacterWidgetInterface->SetupCharacterWidget(this);
	}
}

void UABHpBarWidget::UpdateHpBar(float NewHp)
{
	ensure(MaxHp > 0.f);
	ensure(HpProgressBar != nullptr);

	float HpPercent = NewHp / MaxHp;
	HpProgressBar->SetPercent(HpPercent);
}
