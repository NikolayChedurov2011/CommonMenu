#pragma once

#include "CoreMinimal.h"
#include "CM_ListDataObject_Base.h"
#include "CM_ListDataObject_Value.generated.h"

class FCM_OptionsDataInteractionHelper;

UCLASS(Abstract)
class COMMONMENU_API UCM_ListDataObject_Value : public UCM_ListDataObject_Base
{
	GENERATED_BODY()

public:
	void SetDataDynamicGetter(const TSharedPtr<FCM_OptionsDataInteractionHelper>& DynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FCM_OptionsDataInteractionHelper>& DynamicSetter);

	void SetDefaultValueFromString(const FString& DefaultValue) { DefaultStringValue = DefaultValue; }

	/*-- UCM_ListDataObject_Base --*/
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }
	/*-- End UCM_ListDataObject_Base --*/
protected:
	FString GetDefaultValueAsString() const { return DefaultStringValue.GetValue(); }
	
	TSharedPtr<FCM_OptionsDataInteractionHelper> DataDynamicGetter{};
	TSharedPtr<FCM_OptionsDataInteractionHelper> DataDynamicSetter{};

private:
	TOptional<FString> DefaultStringValue {};
};
