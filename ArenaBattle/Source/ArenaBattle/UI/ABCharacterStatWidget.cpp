#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

UABCharacterStatWidget::UABCharacterStatWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// TFieldIterator - �𸮾� ������ ���÷��� �ý����� �����ϴ� ������� ��ȸ�� �� ����ϴ� �ݺ��� Ŭ����
	// ���÷��� �ý����̶�? ��Ÿ�ӿ� �ڱ� �ڽ��� �����ϴ� ��� (������ �ݷ���, �������Ʈ ���� �� �پ��� ��ɰ� ����)
	// ���÷��� �ý����� �����ϱ� ���ؼ��� UCLASS, USTRUCT, UFUNCTION���� ��ũ�θ� �ٿ��� ��
	// FNumericProperty - ���� Ÿ���� Property�� �����ϱ� ���� ����ϴ� Ŭ���� (������, �Ǽ���)
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName BaseTextBlockName = *FString::Printf(TEXT("Txt_%s_Base"), *PropIt->GetName());
		const FName ModifierTextBlockName = *FString::Printf(TEXT("Txt_%s_Modifier"), *PropIt->GetName());

		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(BaseTextBlockName));
		if (BaseTextBlock)
		{
			BaseTextBlockMap.Add(PropKey, BaseTextBlock);
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(ModifierTextBlockName));
		if (ModifierTextBlock)
		{
			ModifierTextBlockMap.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UABCharacterStatWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	// FABCharacterStat ����ü�� ��� ����� ��ȸ
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		// BaseStat���κ��� ��� ������ ���� ����� ���� ������ ����
		float BaseStatData = 0.f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseStatData);

		// ModifierStat���κ��� ��� ������ ���� ����� ���� ������ ����
		float ModifierStatData = 0.f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierStatData);

		UTextBlock** BaseTextBlockPtr = BaseTextBlockMap.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			// BaseStat�� ǥ������ TextBlock�� ���� ������ ������ ������Ʈ ��
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseStatData)));
		}

		UTextBlock** ModifierTextBlockPtr = ModifierTextBlockMap.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			// ModifierStat�� ǥ������ TextBlock�� ���� ������ ������ ������Ʈ ��
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierStatData)));
		}
	}
}
