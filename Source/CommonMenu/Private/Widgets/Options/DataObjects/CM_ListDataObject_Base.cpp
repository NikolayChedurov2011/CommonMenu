#include "Widgets/Options/DataObjects/CM_ListDataObject_Base.h"

#include "Settings/CM_GameUserSettings.h"

void UCM_ListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UCM_ListDataObject_Base::OnDataObjectInitialized()
{
}

void UCM_ListDataObject_Base::NotifyListDataModified(UCM_ListDataObject_Base* ModifiedData,	EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);
	
	if (bShouldApplyChangeImmediately)
	{
		UCM_GameUserSettings::Get()->ApplySettings(true);
	}
}

void UCM_ListDataObject_Base::AddEditCondition(const FOptionsDataEditConditionDescriptor& EditCondition)
{
	EditConditionDescriptionArray.Add(EditCondition);
}

void UCM_ListDataObject_Base::AddEditDependencyData(UCM_ListDataObject_Base* DependencyData)
{
	if (!DependencyData->OnListDataModified.IsBoundToObject(this))
	{
		DependencyData->OnListDataModified.AddUObject(this, &ThisClass::OnEditDependencyDataModified);
	}
}

bool UCM_ListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsEditable { true };

	if (EditConditionDescriptionArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason{};

	for (const FOptionsDataEditConditionDescriptor& Condition : EditConditionDescriptionArray)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet())
		{
			continue;
		}

		bIsEditable = false;

		CachedDisabledRichReason.Append(Condition.GetDisabledRichReason());

		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue { Condition.GetDisabledForcedStringValue() };

			// If the current value this data object has can be set to the forced value
			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}

	return bIsEditable;
}

void UCM_ListDataObject_Base::OnEditDependencyDataModified(UCM_ListDataObject_Base* ModifiedDependencyData, const EOptionsListDataModifyReason ModifyReason)
{
	OnDependencyDataModified.Broadcast(ModifiedDependencyData, ModifyReason);
}
