// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemDataBase.h"
#include "ABWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABWeaponItemData : public UABItemDataBase
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("ABItemData"), GetFName());
	}

public:
	// 아이템 종류가 많아지면 TObjectPtr로 전부 선로딩(하드 레퍼런싱)할 경우 메모리에 부담을 줄 수 있으므로
	// 애셋 문자열 정보만 가지고 있다가 필요할 때만 로딩하도록 TSoftObjectPtr로 선언함(소프트 레퍼런싱)
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
};
