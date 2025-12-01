#pragma once

#include "CM_StructTypes.generated.h"

USTRUCT()
struct FOptionsDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	void SetEditConditionFunc(const TFunction<bool()>& NewEditConditionFunc)
	{
		EditConditionFunc = NewEditConditionFunc;
	}

	bool IsValid() const
	{
		return EditConditionFunc != nullptr;
	}

	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}

		return true;
	}

	FString GetDisabledRichReason() const { return DisabledRichReason; }
	void SetDisabledRichReason(const FString& RichReason) { DisabledRichReason = RichReason; }

	bool HasForcedStringValue() const {	return DisabledForcedStringValue.IsSet(); }

	FString GetDisabledForcedStringValue() const { return DisabledForcedStringValue.GetValue(); }
	void SetDisabledForcedStringValue(const FString& ForcedValue) { DisabledForcedStringValue = ForcedValue; }
	
private:
	TFunction<bool()> EditConditionFunc{};
	FString DisabledRichReason{};
	TOptional<FString> DisabledForcedStringValue{};
};