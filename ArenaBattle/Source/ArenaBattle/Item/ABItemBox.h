// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AABItemBox();

public:
	FORCEINLINE class UBoxComponent* GetTriggerBox() { return TriggerBox; }

protected:
	virtual void PostInitializeComponents() override;

	// UBoxComponent의 Overlap 이벤트에 bind 해줄 함수
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> TriggerBox;
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> ParticleEffect;

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemDataBase> ItemData;
};
