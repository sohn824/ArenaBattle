// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "ABCharacterNonPlayer.generated.h"


// 'Config/DefaultArenaBattle.ini' ���� ���
UCLASS(config = ArenaBattle)
class ARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase
{
	GENERATED_BODY()
	
public:
	AABCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	virtual void SetDead() override;
	// NpcMesh�� FStreamableHandle�� ���� �ε��� �Ϸ���� �� ȣ���� �ݹ� �Լ�
	void OnNpcMeshLoadCompleted();

protected:
	// 'Config/DefaultArenaBattle.ini' ���� ���
	UPROPERTY(config)
	TArray<FSoftObjectPath> NpcMeshes;

	TSharedPtr<FStreamableHandle> NpcMeshLoadHandle;
};
