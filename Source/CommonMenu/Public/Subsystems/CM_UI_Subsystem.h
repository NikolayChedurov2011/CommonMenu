#pragma once

#include "CoreMinimal.h"
#include "CommonMenuTypes/CM_EnumTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CM_UI_Subsystem.generated.h"

class UCM_ActivableWidget_Base;
class UCM_Layout_Base;
class UCM_CommonButton_Base;
struct FGameplayTag;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDescriptionTextUpdatedDelegate, UCM_CommonButton_Base*, BroadcastingButton, FText, DescriptionText);

UCLASS()
class COMMONMENU_API UCM_UI_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnDescriptionTextUpdatedDelegate OnDescriptionTextUpdated;

	
	static UCM_UI_Subsystem* Get(const UObject* WorldContextObject);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UCM_Layout_Base* Layout);

	void PushSoftWidgetToStackAsync(const FGameplayTag& WidgetStackTag, TSoftClassPtr<UCM_ActivableWidget_Base> SoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UCM_ActivableWidget_Base*)> AsyncPushStateCallback) const;
	void PushConfirmScreenToModalStackAsync(const EConfirmScreenType ScreenType, const FText& ScreenTitle, const FText& ScreenMessage, TFunction<void(EConfirmScreenButtonType)> ButtonCallback) const;

private:

	UPROPERTY(Transient)
	UCM_Layout_Base* RegisteredLayout{};
};
