// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABUserWidget.generated.h"

/**
 * UserWidget을 소유하는 Actor 정보를 얻어오는 기능이
 * 기본 UUserWidget 클래스에는 없으므로 상속받아서 기능 추가
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
