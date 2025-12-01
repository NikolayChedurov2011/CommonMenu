#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"

#include "CM_Layout_Base.generated.h"

class UCommonActivatableWidgetContainerBase;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_Layout_Base : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag StackTag) const;

protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta=(Categories = "UI.WidgetStack")) const FGameplayTag WidgetStackTag, UCommonActivatableWidgetContainerBase* Stack);
	
private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetsStackMap{};
};
