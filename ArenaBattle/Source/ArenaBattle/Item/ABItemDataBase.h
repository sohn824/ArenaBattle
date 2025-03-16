// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemDataBase.generated.h"

/*
 아이템에 필요한 데이터 모음
*/

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll,
};

UCLASS()
class ARENABATTLE_API UABItemDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Warning!
	// 애셋 GetPrimaryAssetId()를 재정의한 경우
	// 해당 클래스를 기반으로 하는 애셋들은 모두 다시 생성해줘야 함
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("ABItemData"), GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType ItemType;
};
