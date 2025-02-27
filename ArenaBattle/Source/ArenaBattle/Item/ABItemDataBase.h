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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType ItemType;
};
