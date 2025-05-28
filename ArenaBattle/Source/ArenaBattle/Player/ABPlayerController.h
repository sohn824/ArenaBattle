#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AABPlayerController();

public:
	void OnPlayerScoreChanged(int32 NewScore);
	void OnGameCleared();
	void OnGameOvered();
	
public:
	// �������Ʈ ȣȯ �Լ���
	// K2�� 'Kismet 2'��� ���̸�, UE�� ���� ���־� ��ũ���� �ý����� Kismet��
	// �ļ� ������ �������Ʈ�̱� ������ �̷� �̸��� �پ���
	// ����, BlueprintImplementableEvent Ű���带 ������ ��� ������ �������Ʈ���� ������ �����ϰڴٴ� �ǹ̷�
	// UE���� �˾Ƽ� Cpp �Լ� ������ �ۼ��ϱ� ������ Cpp������ �Լ� ������ �ۼ��ؼ��� �ȵ�
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnPlayerScoreChangedCpp"))
	void K2_OnPlayerScoreChanged(int32 NewScore);
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearedCpp"))
	void K2_OnGameCleared();
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOveredCpp"))
	void K2_OnGameOvered();

protected:
	virtual void BeginPlay() override;

/// HUD(Head-Up Display) Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UABHUDWidget> ABHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UABHUDWidget> ABHUDWidgetPtr;
};
