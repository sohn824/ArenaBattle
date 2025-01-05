// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	ShoulderView,
	QuaterView,
};

UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AABCharacterBase();

protected:
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlDataMap;

	// �ִϸ��̼� ��Ÿ�ָ� �̿��� �޺� ���μ��� ����
protected:
	void ComboActionProcess();

	// �ִϸ��̼� ��Ÿ�ֿ��� ȣ���� �ݹ� �Լ���
	void ComboActionBeginCallback();
	void ComboActionEndCallback(class UAnimMontage* TargetMontage, bool bIsProperlyEnded); // FOnMontageEnded ��������Ʈ ���� ����

	// �޺� �Է� üũ �Լ���
	void ResetComboCheckTimer();
	void ComboTimerProc();

	// �ִϸ��̼� ��Ÿ�ִ� �����Ϳ��� �������Ʈ�� �����ϵ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// �޺� �׼ǿ� �ʿ��� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	int32 CurrentComboCount = 0;
	FTimerHandle ComboTimerHandle;
	bool bIsNextComboCommandInputed = false;
};
