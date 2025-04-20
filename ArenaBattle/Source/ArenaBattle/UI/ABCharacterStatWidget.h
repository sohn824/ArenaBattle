#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTable/ABCharacterStat.h"
#include "ABCharacterStatWidget.generated.h"

/*
  캐릭터 스탯을 표기하기 위한 UI Widget 클래스
  (ABHUDWidget의 하위 위젯, ABHUDWidget으로부터 스탯 정보를 받아 업데이트함)
*/
UCLASS()
class ARENABATTLE_API UABCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UABCharacterStatWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);

private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseTextBlockMap;
	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierTextBlockMap;
};
