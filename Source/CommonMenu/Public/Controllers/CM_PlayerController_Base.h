#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CM_PlayerController_Base.generated.h"

UCLASS()
class COMMONMENU_API ACM_PlayerController_Base : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
