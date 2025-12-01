#pragma once

#include "CoreMinimal.h"
#include "CommonMenuTypes/CM_EnumTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CM_AsyncAction_PushConfirmScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedDelegate, EConfirmScreenButtonType, ClickedButtonType);

UCLASS()
class COMMONMENU_API UCM_AsyncAction_PushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked;

	/*-- UBlueprintAsyncActionBase Interface --*/
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", HidePin="WorldContextObject", BlueprintInternalUseOnly="true", DisplayName="Show Confirmation Screen"))
	static UCM_AsyncAction_PushConfirmScreen* PushConfirmScreen(const UObject* WorldContextObject, EConfirmScreenType ScreenType, FText ScreenTitle, FText ScreenMessage);

private:
	TWeakObjectPtr<UWorld> CachedWorld {};
	EConfirmScreenType CachedConfirmScreenType {};
	FText CachedScreenTitle {};
	FText CachedScreenMessage {};
};
