#include "Widgets/Options/DataObjects/CM_ListDataObject_Scalar.h"

#include "Widgets/Options/CM_OptionsDataInteractionHelper.h"

FCommonNumberFormattingOptions UCM_ListDataObject_Scalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;

	return Options;
}

FCommonNumberFormattingOptions UCM_ListDataObject_Scalar::WithDecimal(int32 NumFracDigit)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigit;

	return Options;
}

float UCM_ListDataObject_Scalar::GetCurrentValue() const
{
	if (!DataDynamicGetter.IsValid())
	{
		return 0.0f;
	}

	return FMath::GetMappedRangeValueClamped(OutputValueRange, DisplayValueRange, StringToFloat(DataDynamicGetter->GetValueAsString()));
}

void UCM_ListDataObject_Scalar::SetCurrentValueFromSlider(const float NewValue)
{
	if (!DataDynamicSetter.IsValid())
	{
		return;
	}

	const float ClampedValue { FMath::GetMappedRangeValueClamped(DisplayValueRange, OutputValueRange, NewValue) };
	DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));

	NotifyListDataModified(this);
}

bool UCM_ListDataObject_Scalar::CanResetBackToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicGetter.IsValid())
	{
		const float DefaultValue { StringToFloat(GetDefaultValueAsString()) };
		const float CurrentValue { StringToFloat(DataDynamicGetter->GetValueAsString()) };

		return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.1f);
	}

	return false;
}

bool UCM_ListDataObject_Scalar::TryResetBackToDefaultValue()
{
	if (!CanResetBackToDefaultValue() && !DataDynamicGetter.IsValid())
	{
		return false;
	}

	DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());
	
	NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

	return true;
}

void UCM_ListDataObject_Scalar::OnEditDependencyDataModified(UCM_ListDataObject_Base* ModifiedDependencyData, const EOptionsListDataModifyReason ModifyReason)
{
	NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	
	Super::OnEditDependencyDataModified(ModifiedDependencyData, ModifyReason);
}

float UCM_ListDataObject_Scalar::StringToFloat(const FString& String) const
{
	float OutConvertedValue { 0.f };
	LexFromString(OutConvertedValue, *String);

	return OutConvertedValue;
}
