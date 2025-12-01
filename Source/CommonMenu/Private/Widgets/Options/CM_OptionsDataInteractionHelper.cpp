#include "Widgets/Options/CM_OptionsDataInteractionHelper.h"

#include "Settings/CM_GameUserSettings.h"

FCM_OptionsDataInteractionHelper::FCM_OptionsDataInteractionHelper(const FString& SetterOrGetterFuncPath)
	: CachedDynamicFunctionPath(SetterOrGetterFuncPath)
{
	CachedWeakGameUserSettings = UCM_GameUserSettings::Get();
}

FString FCM_OptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue{};
	PropertyPathHelpers::GetPropertyValueAsString(CachedWeakGameUserSettings.Get(), CachedDynamicFunctionPath, OutStringValue);

	return OutStringValue;
}

void FCM_OptionsDataInteractionHelper::SetValueFromString(const FString& StringValue) const
{
	PropertyPathHelpers::SetPropertyValueFromString(CachedWeakGameUserSettings.Get(), CachedDynamicFunctionPath, StringValue);
}
