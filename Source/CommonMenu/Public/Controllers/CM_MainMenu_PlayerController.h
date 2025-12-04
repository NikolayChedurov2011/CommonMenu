#pragma once

#include "CoreMinimal.h"
#include "CM_MainMenu_PlayerController.generated.h"

UCLASS()
class COMMONMENU_API ACM_MainMenu_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
