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
	// 블루프린트 호환 함수들
	// K2는 'Kismet 2'라는 뜻이며, UE의 예전 비주얼 스크립팅 시스템인 Kismet의
	// 후속 버전이 블루프린트이기 때문에 이런 이름이 붙었음
	// 또한, BlueprintImplementableEvent 키워드를 지정할 경우 오로지 블루프린트에서 내용을 구현하겠다는 의미로
	// UE에서 알아서 Cpp 함수 본문을 작성하기 때문에 Cpp에서는 함수 본문을 작성해서는 안됨
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
