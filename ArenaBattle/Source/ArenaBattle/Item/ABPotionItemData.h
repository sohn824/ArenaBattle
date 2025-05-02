#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemDataBase.h"
#include "ABPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABPotionItemData : public UABItemDataBase
{
	GENERATED_BODY()
public:
	UABPotionItemData();

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("ABItemData"), GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Hp)
	float HealAmount;
};
