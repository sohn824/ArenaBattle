#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTable/ABCharacterStat.h"
#include "ABCharacterStatWidget.generated.h"

/*
  ĳ���� ������ ǥ���ϱ� ���� UI Widget Ŭ����
  (ABHUDWidget�� ���� ����, ABHUDWidget���κ��� ���� ������ �޾� ������Ʈ��)
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
