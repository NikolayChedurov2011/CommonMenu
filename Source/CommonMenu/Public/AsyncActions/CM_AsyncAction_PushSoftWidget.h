#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CM_AsyncAction_PushSoftWidget.generated.h"

class UCM_ActivableWidget_Base;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UCM_ActivableWidget_Base*, PushedWidget);

UCLASS()
class COMMONMENU_API UCM_AsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate AfterPush;

	/*-- UBlueprintAsyncActionBase Interface --*/
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", HidePin="WorldContextObject", BlueprintInternalUseOnly="true", DisplayName="Push Soft Widget To Widget Stack"))
	static UCM_AsyncAction_PushSoftWidget* PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController,
		TSoftClassPtr<UCM_ActivableWidget_Base> SoftWidgetClass, UPARAM(meta=(Categories = "UI.WidgetStack")) const FGameplayTag WidgetStackTag, bool bFocusOnCreatedWidget = true);

private:
	TWeakObjectPtr<UWorld> CachedWorld {};
	TWeakObjectPtr<APlayerController> CachedPlayerController {};
	TSoftClassPtr<UCM_ActivableWidget_Base> CachedSoftWidgetClass {};
	FGameplayTag CachedWidgetStackTag {};
	bool bCachedFocusOnCreatedWidget = false;
};
