#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UCM_GameUserSettings;

class COMMONMENU_API FCM_OptionsDataInteractionHelper
{
public:
	FCM_OptionsDataInteractionHelper(const FString& SetterOrGetterFuncPath);

	FString GetValueAsString() const;
	void SetValueFromString(const FString& StringValue) const;

private:
	FCachedPropertyPath CachedDynamicFunctionPath{};
	TWeakObjectPtr<UCM_GameUserSettings> CachedWeakGameUserSettings{};
};
