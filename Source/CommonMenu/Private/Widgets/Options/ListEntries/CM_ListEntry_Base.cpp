#include "Widgets/Options/ListEntries/CM_ListEntry_Base.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Widgets/Components/CM_CommonListView.h"
#include "Widgets/Options/DataObjects/CM_ListDataObject_Base.h"

void UCM_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UCM_ListDataObject_Base>(ListItemObject));
}

void UCM_ListEntry_Base::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	BP_OnToggleEntryWidgetHighlightState(bIsSelected);
}

void UCM_ListEntry_Base::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	NativeOnListEntryWidgetHovered(false);
}

FReply UCM_ListEntry_Base::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	const UCommonInputSubsystem* CommonInputSubsystem { GetInputSubsystem() };

	if (CommonInputSubsystem && CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		const UWidget* WidgetToFocus { BP_GetWidgetToFocusForGamepad() };

		if (WidgetToFocus)
		{
			const TSharedPtr<SWidget> SlateWidgetToFocus { WidgetToFocus->GetCachedWidget() };

			if (SlateWidgetToFocus)
			{
				return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
			}
		}
	}

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UCM_ListEntry_Base::OnOwningListDataObjectSet(UCM_ListDataObject_Base* OwningListDataObject)
{
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetText(OwningListDataObject->GetDataDisplayName());
	}
	
	if (!OwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		OwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}

	if (!OwningListDataObject->OnDependencyDataModified.IsBoundToObject(this))
	{
		OwningListDataObject->OnDependencyDataModified.AddUObject(this, &ThisClass::OnOwningDependencyDataObjectModified);
	}
	
	OnToggleEditableState(OwningListDataObject->IsDataCurrentlyEditable());

	CachedOwningDataObject = OwningListDataObject;
}

void UCM_ListEntry_Base::OnOwningListDataObjectModified(UCM_ListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	
}

void UCM_ListEntry_Base::OnOwningDependencyDataObjectModified(UCM_ListDataObject_Base* OwningModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningDataObject)
	{
		OnToggleEditableState(CachedOwningDataObject->IsDataCurrentlyEditable());
	}
}

void UCM_ListEntry_Base::OnToggleEditableState(const bool bIsEditable)
{
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetIsEnabled(bIsEditable);
	}
}

void UCM_ListEntry_Base::SelectThisEntryWidget() const
{
	CastChecked<UCM_CommonListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}

void UCM_ListEntry_Base::NativeOnListEntryWidgetHovered(const bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, GetListItem()? IsListItemSelected() : false);

	if (bWasHovered)
	{
		BP_OnToggleEntryWidgetHighlightState(true);
	}
	else
	{
		BP_OnToggleEntryWidgetHighlightState(GetListItem() && IsListItemSelected()? true : false);
	}
}
