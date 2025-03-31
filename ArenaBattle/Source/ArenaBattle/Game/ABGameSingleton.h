#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataTable/ABCharacterStat.h"
#include "ABGameSingleton.generated.h"

UCLASS()
class ARENABATTLE_API UABGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UABGameSingleton();

	static UABGameSingleton& GetInstance();

public:
	FABCharacterStat GetCharacterStat(int32 InLevel)
	{
		return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FABCharacterStat();
	}

public:
	UPROPERTY()
	int32 CharacterMaxLevel = 0;

private:
	TArray<FABCharacterStat> CharacterStatTable;
};
