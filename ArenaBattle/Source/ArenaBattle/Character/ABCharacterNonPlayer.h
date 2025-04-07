// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/ABCharacterAIInterface.h"
#include "ABCharacterNonPlayer.generated.h"


// 'Config/DefaultArenaBattle.ini' ���� ���
UCLASS(config = ArenaBattle)
class ARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase, public IABCharacterAIInterface
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

	// IABCharacterAIInterface
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAICharacterAttackFinishedDelegate(const FOnAICharacterAttackFinished& InDelegate) override;
	virtual void AttackByAI() override;

	FOnAICharacterAttackFinished OnAttackFinished;

protected:
	virtual void OnComboActionEnd() override;
};
