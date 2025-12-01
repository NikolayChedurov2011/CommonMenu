#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CM_ActivableWidget_Base.generated.h"

class ACM_PlayerController_Base;

UCLASS()
class COMMONMENU_API UCM_ActivableWidget_Base : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	ACM_PlayerController_Base* GetOwningCommonMenuPlayerController();
	
private:
	TSoftObjectPtr<ACM_PlayerController_Base> CachedOwningPlayerController{};
};
