// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "ABCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	ShoulderView,
	QuaterView,
};

UCLASS()
class ARENABATTLE_API AABCharacterBase
	: public ACharacter
	, public IABAnimationAttackInterface
	, public IABCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	AABCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlDataMap;

/// Combo Process Section
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

/// Attack & Damage Section
protected:
	virtual void AttackHitCheck() override; // UABAnimationAttackInterface
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
/// Dead Section
protected:
	// �ִϸ��̼� ��Ÿ�ִ� �����Ϳ��� �������Ʈ�� �����ϵ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

/// Character Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> StatComponent;

/// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBarComponent;

	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget) override; //IABCharacterWidgetInterface
};
