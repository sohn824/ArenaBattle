// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemDataBase.h"
#include "DataTable/ABCharacterStat.h"
#include "ABWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABWeaponItemData : public UABItemDataBase
{
	GENERATED_BODY()
public:
	UABWeaponItemData();

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("ABItemData"), GetFName());
	}

public:
	// ������ ������ �������� TObjectPtr�� ���� ���ε�(�ϵ� ���۷���)�� ��� �޸𸮿� �δ��� �� �� �����Ƿ�
	// �ּ� ���ڿ� ������ ������ �ִٰ� �ʿ��� ���� �ε��ϵ��� TSoftObjectPtr�� ������(����Ʈ ���۷���)
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	// �� �������� ���������ν� ĳ���Ϳ� ������ ���� ������
	UPROPERTY(EditAnywhere, Category = Stat)
	FABCharacterStat ModifierStat;
};
