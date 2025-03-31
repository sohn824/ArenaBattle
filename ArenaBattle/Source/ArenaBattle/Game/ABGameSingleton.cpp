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
		// RowMap�� ��� Value�� TArray�� ����
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		// Algo::Transform() -> �����̳��� �����͸� ��ȯ�Ͽ� �ٸ� �����̳ʿ� ����
		// @ Input : ��ȯ�� �����̳�
		// @ Output : ��ȯ ����� ���� �����̳�
		// @ Trans : ��ȯ �Լ� (���� ǥ���� ����)
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				// ValueArray�� ���ҵ��� FABCharacterStat �������� ���� ����ȯ�Ͽ� CharacterStatTable�� ����
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
