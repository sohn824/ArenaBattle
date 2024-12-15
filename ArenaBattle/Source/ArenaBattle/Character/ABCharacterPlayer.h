// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "InputActionValue.h"
#include "ABCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase
{
	GENERATED_BODY()
	
public:
	AABCharacterPlayer();

protected:
	virtual void BeginPlay() override;
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;	// USpringArmComponent - 카메라 지지대 역할
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;	// UCameraComponent - 실제 카메라 역할

	// Input Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	// InputAction과 매핑할 함수들
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
};
