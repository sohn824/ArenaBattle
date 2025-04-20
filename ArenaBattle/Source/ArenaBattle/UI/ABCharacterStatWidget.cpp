#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

UABCharacterStatWidget::UABCharacterStatWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// TFieldIterator - 언리얼 엔진의 리플렉션 시스템을 지원하는 멤버들을 순회할 때 사용하는 반복자 클래스
	// 리플렉션 시스템이란? 런타임에 자기 자신을 조사하는 기능 (가비지 콜렉션, 블루프린트 연동 등 다양한 기능과 연동)
	// 리플렉션 시스템을 지원하기 위해서는 UCLASS, USTRUCT, UFUNCTION등의 매크로를 붙여야 함
	// FNumericProperty - 숫자 타입의 Property를 관리하기 위해 사용하는 클래스 (정수형, 실수형)
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
	// FABCharacterStat 구조체의 모든 멤버를 순회
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		// BaseStat으로부터 멤버 데이터 값을 갖고와 지역 변수에 저장
		float BaseStatData = 0.f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseStatData);

		// ModifierStat으로부터 멤버 데이터 값을 갖고와 지역 변수에 저장
		float ModifierStatData = 0.f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierStatData);

		UTextBlock** BaseTextBlockPtr = BaseTextBlockMap.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			// BaseStat을 표시해줄 TextBlock의 값을 가져온 값으로 업데이트 함
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseStatData)));
		}

		UTextBlock** ModifierTextBlockPtr = ModifierTextBlockMap.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			// ModifierStat을 표시해줄 TextBlock의 값을 가져온 값으로 업데이트 함
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierStatData)));
		}
	}
}
