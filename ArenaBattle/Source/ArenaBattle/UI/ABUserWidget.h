// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABUserWidget.generated.h"

/**
 * UserWidget�� �����ϴ� Actor ������ ������ �����
 * �⺻ UUserWidget Ŭ�������� �����Ƿ� ��ӹ޾Ƽ� ��� �߰�
 */
UCLASS()
class ARENABATTLE_API UABUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOwnerActor(AActor* NewOwner) { OwnerActor = NewOwner; }
	FORCEINLINE AActor* GetOwnerActor() { return OwnerActor; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Actor)
	TObjectPtr<AActor> OwnerActor;
};
