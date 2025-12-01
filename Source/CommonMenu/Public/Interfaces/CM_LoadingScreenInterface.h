#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CM_LoadingScreenInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UCM_LoadingScreenInterface : public UInterface
{
	GENERATED_BODY()
};

class COMMONMENU_API ICM_LoadingScreenInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenActivated();
	/*virtual void OnLoadingScreenActivated_Implementation();*/

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenDeactivated();
	/*virtual void OnLoadingScreenDeactivated_Implementation();*/
};
