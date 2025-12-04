#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CM_ActivableWidget_Base.generated.h"

UCLASS()
class COMMONMENU_API UCM_ActivableWidget_Base : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	APlayerController* GetOwningCommonMenuPlayerController();
	
private:
	TSoftObjectPtr<APlayerController> CachedOwningPlayerController{};
};
