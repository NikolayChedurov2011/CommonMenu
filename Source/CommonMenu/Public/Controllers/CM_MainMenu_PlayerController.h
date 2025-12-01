#pragma once

#include "CoreMinimal.h"
#include "CM_PlayerController_Base.h"
#include "CM_MainMenu_PlayerController.generated.h"

UCLASS()
class COMMONMENU_API ACM_MainMenu_PlayerController : public ACM_PlayerController_Base
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
