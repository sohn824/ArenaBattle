// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UABCharacterControlData();

	// 컨트롤러의 회전 Yaw값 사용 여부 (3인칭 시점에서 주로 사용)
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	// 움직이는 방향으로 캐릭터를 자동으로 회전시킬 것인지
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;
	// 캐릭터 회전 시에 목표 회전각으로 부드럽게 회전시킬 것인지
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
};
