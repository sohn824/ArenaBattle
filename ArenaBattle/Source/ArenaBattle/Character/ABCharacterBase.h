#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "Interface/ABCharacterItemInterface.h"
#include "ABCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	ShoulderView,
	QuaterView,
};

// 아이템을 획득했을 때 호출할 콜백함수 Delegate 선언
DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemDataBase*);
// Delegate를 배열에 담아 관리하기 위해 구조체로 감싼다.
USTRUCT(BlueprintType)
struct FOnTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FOnTakeItemDelegateWrapper() {}
	FOnTakeItemDelegateWrapper(const FOnTakeItemDelegate& InDelegate) : Delegate(InDelegate) {}

	FOnTakeItemDelegate Delegate;
};

UCLASS()
class ARENABATTLE_API AABCharacterBase
	: public ACharacter
	, public IABAnimationAttackInterface
	, public IABCharacterWidgetInterface
	, public IABCharacterItemInterface
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

	// 애니메이션 몽타주가 시작될 때 호출할 콜백 함수
	void ComboActionBeginCallback();

	// 애니메이션 몽타주가 끝날 때 호출할 콜백 함수 (FOnMontageEnded 델리게이트 형식 맞춤)
	void ComboActionEndCallback(class UAnimMontage* TargetMontage, bool bIsProperlyEnded);

	// ComboActionEndCallback이 호출될 때 추가로 호출해 줄 콜백 함수
	// 필요할 경우 이 클래스를 상속받은 클래스들이 이 함수를 override하여 각자 다른 처리를 할 수 있도록 하기 위하여 추가
	virtual void OnComboActionEnd();

	// 콤보 입력 체크 함수들
	void ResetComboCheckTimer();
	void ComboTimerProc();

	// 애니메이션 몽타주는 에디터에서 블루프린트로 설정하도록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// 콤보 액션에 필요한 데이터 에셋
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
	// 애니메이션 몽타주는 에디터에서 블루프린트로 설정하도록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

/// Character Stat Section
public:
	int32 GetCharacterLevel();
	void SetCharacterLevel(int32 NewLevel);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> StatComponent;

/// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBarComponent;

	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget) override; //IABCharacterWidgetInterface

/// Item Section
protected:
	UPROPERTY()
	TArray<FOnTakeItemDelegateWrapper> TakeItemDelegates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

	virtual void TakeItem(class UABItemDataBase* InItemData) override; //IABCharacterItemInterface

	// Weapon
	virtual void EquipWeapon(class UABItemDataBase* InItemData);
	// Potion
	virtual void DrinkPotion(class UABItemDataBase* InItemData);
	// Scroll
	virtual void ReadScroll(class UABItemDataBase* InItemData);
};
