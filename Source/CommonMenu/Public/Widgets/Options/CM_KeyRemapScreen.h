#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Widgets/CM_ActivableWidget_Base.h"
#include "CM_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_KeyRemapScreen : public UCM_ActivableWidget_Base
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /*PressedKey*/);
	FOnKeyRemapScreenKeyPressedDelegate OnKeyRemapScreenKeyPressed;
	
	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCanceledDelegate, const FString& /*CanceledReason*/);
	FOnKeyRemapScreenKeySelectCanceledDelegate OnKeyRemapScreenKeySelectCanceled;
	
	void SetDesiredInputTypeToFilter(const ECommonInputType DesiredInputType);

protected:
	/*-- UCommonActivableWidget --*/
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	/*-- End UCommonActivableWidget --*/

private:
	void OnValidKeyPressedDetected(const FKey& PressedKey);
	void OnKeySelectCancelled(const FString& CanceledReason);

	// Delay a tick to make sure the input key is captured properly before calling the PreDeactivateCallback and deactivating the widget
	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);
	
	/*-- Bound Widgets --*/
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichText_RemapMessage{};
	/*-- End Bound Widgets --*/

	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreprocessor{};

	ECommonInputType CachedDesiredInputType{};
};
