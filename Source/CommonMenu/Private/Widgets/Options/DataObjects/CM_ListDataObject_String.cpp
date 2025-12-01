#include "Widgets/Options/DataObjects/CM_ListDataObject_String.h"

#include "Widgets/Options/CM_OptionsDataInteractionHelper.h"

void UCM_ListDataObject_String::AddDynamicOption(const FString& StringValue, const FText& DisplayText)
{
	AvailableOptionsStringArray.Add(StringValue);
	AvailableOptionsTextArray.Add(DisplayText);
}

void UCM_ListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}
	
	if (DataDynamicGetter.IsValid())
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
	}
}

bool UCM_ListDataObject_String::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UCM_ListDataObject_String::TryResetBackToDefaultValue()
{
	if (!CanResetBackToDefaultValue())
	{
		return false;
	}

	CurrentStringValue = GetDefaultValueAsString();
	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);
		return true;
	}

	return false;
}

bool UCM_ListDataObject_String::CanSetToForcedStringValue(const FString& ForcedValue) const
{
	return CurrentStringValue != ForcedValue;
}

void UCM_ListDataObject_String::OnSetToForcedStringValue(const FString& ForcedValue)
{
	CurrentStringValue = ForcedValue;

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter.IsValid())
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}
}

bool UCM_ListDataObject_String::TrySetDisplayTextFromStringValue(const FString& StringValue)
{
	const int32 CurrentFoundIndex { AvailableOptionsStringArray.IndexOfByKey(StringValue) };

	if (AvailableOptionsTextArray.IsValidIndex(CurrentFoundIndex))
	{
		CurrentDisplayText = AvailableOptionsTextArray[CurrentFoundIndex];

		return true;
	}

	return false;
}

void UCM_ListDataObject_String::AdvanceToNextOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex { AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue) };
	const int32 NextIndexToDisplay { CurrentDisplayIndex + 1 };

	const bool bIsNextIndexValid = AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay);

	if (bIsNextIndexValid)
	{
		CurrentStringValue = AvailableOptionsStringArray[NextIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter.IsValid())
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		NotifyListDataModified(this);
	}
}

void UCM_ListDataObject_String::BackToPreviousOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex { AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue) };
	const int32 PreviousIndexToDisplay { CurrentDisplayIndex - 1 };

	const bool bIsNextIndexValid = AvailableOptionsStringArray.IsValidIndex(PreviousIndexToDisplay);

	if (bIsNextIndexValid)
	{
		CurrentStringValue = AvailableOptionsStringArray[PreviousIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray.Last();
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);
	
	if (DataDynamicSetter.IsValid())
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		NotifyListDataModified(this);
	}
}

void UCM_ListDataObject_String::OnRotatorInitiatedValueChange(const FText& NewSelectedText)
{
	const int32 FoundText = AvailableOptionsTextArray.IndexOfByPredicate([NewSelectedText](const FText& AvailableText)->bool
	{
		return AvailableText.EqualTo(NewSelectedText);
	});

	if (FoundText == INDEX_NONE || !AvailableOptionsStringArray.IsValidIndex(FoundText))
	{
		return;
	}

	CurrentDisplayText = NewSelectedText;
	CurrentStringValue = AvailableOptionsStringArray[FoundText];

	if (DataDynamicSetter.IsValid())
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this);
	}
}

//////////////////////////////
// StringBool
void UCM_ListDataObject_StringBool::OnDataObjectInitialized()
{
	TryInitBoolValue();
	
	Super::OnDataObjectInitialized();
}

void UCM_ListDataObject_StringBool::OverrideTrueDisplayText(const FText& NewTrueDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString, NewTrueDisplayText);
	}
}

void UCM_ListDataObject_StringBool::OverrideFalseDisplayText(const FText& NewFalseDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, NewFalseDisplayText);
	}
}

void UCM_ListDataObject_StringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(TrueString);
}

void UCM_ListDataObject_StringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(FalseString);
}

void UCM_ListDataObject_StringBool::TryInitBoolValue()
{
	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString, FText::FromString(TEXT("ON")));
	}

	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, FText::FromString(TEXT("OFF")));
	}
}

//////////////////////////////
// Integer
void UCM_ListDataObject_StringInteger::AddIntegerOption(const int32 IntegerValue, const FText& DisplayText)
{
	AddDynamicOption(LexToString(IntegerValue), DisplayText);
}

void UCM_ListDataObject_StringInteger::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Custom"));
	}
}

void UCM_ListDataObject_StringInteger::OnEditDependencyDataModified(UCM_ListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	if (DataDynamicGetter.IsValid())
	{
		if (DataDynamicGetter->GetValueAsString().Equals(CurrentStringValue))
		{
			return;
		}
		
		CurrentStringValue = DataDynamicGetter->GetValueAsString();

		if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
		{
			CurrentDisplayText = FText::FromString(TEXT("Custom"));
		}

		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}

	Super::OnEditDependencyDataModified(ModifiedDependencyData, ModifyReason);
}
