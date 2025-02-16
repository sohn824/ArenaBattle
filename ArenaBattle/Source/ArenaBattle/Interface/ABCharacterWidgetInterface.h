#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ABCharacterWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UABCharacterWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/*
 캐릭터에 UABUserWidget을 붙이고 싶을 때 사용할 인터페이스 
 */
class ARENABATTLE_API IABCharacterWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget);
};
