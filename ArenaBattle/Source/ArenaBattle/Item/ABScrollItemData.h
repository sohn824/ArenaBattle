#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemDataBase.h"
#include "DataTable/ABCharacterStat.h"
#include "ABScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABScrollItemData : public UABItemDataBase
{
	GENERATED_BODY()

public:
	UABScrollItemData();

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("ABItemData"), GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FABCharacterStat BonusBaseStat;
};
