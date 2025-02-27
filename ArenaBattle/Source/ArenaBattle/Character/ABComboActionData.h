#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/*
 �޺� �׼� ������ �ʿ��� ������ ����
*/
UCLASS()
class ARENABATTLE_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UABComboActionData();

	// �ִϸ��̼� ��Ÿ�� ���� �̸� ���λ�
	UPROPERTY(EditAnywhere, Category = ComboData)
	FString MontageSectionNamePrefix;
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint8 MaxComboCount;
	UPROPERTY(EditAnywhere, Category = ComboData)
	float AnimationFrameRate;
	// �� �޺� �ִϸ��̼Ǹ��� �Է� ���� ó���� ������ ������ �ε��� ����
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameIndexes;
};
