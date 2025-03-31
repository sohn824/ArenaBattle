// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ABGameSingleton.h"
#include "DataTable/ABCharacterStat.h"

UABGameSingleton::UABGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(
		TEXT("/Script/Engine.DataTable'/Game/ArenaBattle/DataTable/ABCharacterStatTable.ABCharacterStatTable'"));
	if (DataTableRef.Object != nullptr)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		// RowMap의 모든 Value를 TArray에 복사
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		// Algo::Transform() -> 컨테이너의 데이터를 변환하여 다른 컨테이너에 저장
		// @ Input : 변환할 컨테이너
		// @ Output : 변환 결과를 담을 컨테이너
		// @ Trans : 변환 함수 (람다 표현식 가능)
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				// ValueArray의 원소들을 FABCharacterStat 형식으로 강제 형변환하여 CharacterStatTable에 넣음
				FABCharacterStat* TransformedData = reinterpret_cast<FABCharacterStat*>(Value);
				return *TransformedData;
			});
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UABGameSingleton& UABGameSingleton::GetInstance()
{
	UABGameSingleton* Instance = CastChecked<UABGameSingleton>(GEngine->GameSingleton);
	if (Instance != nullptr)
	{
		return *Instance;
	}

	UE_LOG(LogTemp, Error, TEXT("UABGameSingleton Invalid!"));
	return *NewObject<UABGameSingleton>();
}
