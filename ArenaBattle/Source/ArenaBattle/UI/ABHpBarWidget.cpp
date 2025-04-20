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
	// OwnerActor�� IABCharacterWidgetInterface�� ������ �ִ��� �˻�
	if (CharacterWidgetInterface)
	{
		// IABCharacterWidgetInterface�� ������ �ִٸ� OwnerActor�� HpBarWidget ����
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
