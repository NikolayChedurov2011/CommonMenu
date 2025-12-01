#pragma once

#include "CoreMinimal.h"
#include "Widgets/CM_ActivableWidget_Base.h"
#include "CommonMenuTypes/CM_EnumTypes.h"
#include "CM_ConfirmationScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType{EConfirmScreenButtonType::Unknown};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TextToDisplay{};
};

UCLASS()
class COMMONMENU_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	static UConfirmScreenInfoObject* CreateOKScreen(const FText& ScreenTitle, const FText& ScreenMessage);
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& ScreenTitle, const FText& ScreenMessage);
	static UConfirmScreenInfoObject* CreateOkCancelScreen(const FText& ScreenTitle, const FText& ScreenMessage);
	
	UPROPERTY(Transient)
	FText CachedScreenTitle{};

	UPROPERTY(Transient)
	FText CachedScreenMessage{};

	UPROPERTY(Transient)
	TArray<FConfirmScreenButtonInfo> AvailableScreenButtons{};
};

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_ConfirmationScreen : public UCM_ActivableWidget_Base
{
	GENERATED_BODY()

public:
	// Gets called outside the class when this widget is constructed, and before it's pushed to the modal stack
	void InitConfirmScreen(const UConfirmScreenInfoObject* ScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ButtonCallback);
	
protected:
	/*-- UCommonActivableWidget --*/
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	/*-- End UCommonActivableWidget --*/
	
private:
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;

	UPROPERTY(meta=(BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons; 
};
