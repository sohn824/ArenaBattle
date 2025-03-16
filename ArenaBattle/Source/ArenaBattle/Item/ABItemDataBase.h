// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemDataBase.generated.h"

/*
 �����ۿ� �ʿ��� ������ ����
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
	// �ּ� GetPrimaryAssetId()�� �������� ���
	// �ش� Ŭ������ ������� �ϴ� �ּµ��� ��� �ٽ� ��������� ��
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("ABItemData"), GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType ItemType;
};
