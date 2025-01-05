// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UABComboActionData();

	// 애니메이션 몽타주 섹션 이름 접두사
	UPROPERTY(EditAnywhere, Category = ComboData)
	FString MontageSectionNamePrefix;
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint8 MaxComboCount;
	UPROPERTY(EditAnywhere, Category = ComboData)
	float AnimationFrameRate;
	// 각 콤보 애니메이션마다 입력 성공 처리를 시작할 프레임 인덱스 모음
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameIndexes;
};
