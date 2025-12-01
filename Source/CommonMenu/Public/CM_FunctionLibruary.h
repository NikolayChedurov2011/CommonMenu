#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CM_FunctionLibruary.generated.h"

class UCM_ActivableWidget_Base;

UCLASS()
class COMMONMENU_API UCM_FunctionLibruary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Common Menu Function Libruary")
	static TSoftClassPtr<UCM_ActivableWidget_Base> GetSoftWidgetClassByTag(UPARAM(meta=(Categories = "UI.Widget")) const FGameplayTag WidgetTag);

	UFUNCTION(BlueprintPure, Category = "Common Menu Function Libruary")
	static TSoftObjectPtr<UTexture2D> GetOptionsSoftImageByTag(UPARAM(meta=(Categories = "UI.Image")) const FGameplayTag ImageTag);
};
