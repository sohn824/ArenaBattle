// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ABWidgetComponent.generated.h"

/**
 UWidgetComponent�� OwnerActor ������ �� �� �ֵ��� Ȯ���� Ŭ���� 
 */
UCLASS()
class ARENABATTLE_API UABWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void InitWidget() override;
};
