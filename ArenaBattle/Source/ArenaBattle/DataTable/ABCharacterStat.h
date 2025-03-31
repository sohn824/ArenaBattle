#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ABCharacterStat.generated.h"

// Excel ������ �о���̱� ���� ����ü
// FTableRowBase�� ��ӹ��� ����ü�� .csv ������ Excel ������ import�� �� ����
USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterStat()
		: MaxHp(0.f)
		, AttackDamage(0.f)
		, AttackRange(0.f)
		, AttackSpeed(0.f)
		, MovementSpeed(0.f)
	{ }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	FABCharacterStat operator+(const FABCharacterStat& OtherStat) const
	{
		FABCharacterStat Result;
		Result.MaxHp = this->MaxHp + OtherStat.MaxHp;
		Result.AttackDamage = this->MaxHp + OtherStat.AttackDamage;
		Result.AttackRange = this->AttackRange + OtherStat.AttackRange;
		Result.AttackSpeed = this->AttackSpeed + OtherStat.AttackSpeed;
		Result.MovementSpeed = this->MovementSpeed + OtherStat.MovementSpeed;

		return Result;
	}
};