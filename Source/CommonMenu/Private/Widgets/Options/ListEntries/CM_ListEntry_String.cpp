#include "Widgets/Options/ListEntries/CM_ListEntry_String.h"

#include "CommonInputSubsystem.h"
#include "Widgets/Components/CM_CommonButton_Base.h"
#include "Widgets/Components/CM_CommonRotator.h"
#include "Widgets/Options/DataObjects/CM_ListDataObject_String.h"

void UCM_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousOptionButtonClicked);
	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextOptionButtonClicked);

	CommonRotator_AvailableOptions->OnClicked().AddLambda([this]()
	{
		SelectThisEntryWidget();
	});
	CommonRotator_AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);
}

void UCM_ListEntry_String::OnOwningListDataObjectSet(UCM_ListDataObject_Base* OwningListDataObject)
{
	Super::OnOwningListDataObjectSet(OwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UCM_ListDataObject_String>(OwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}

void UCM_ListEntry_String::OnOwningListDataObjectModified(UCM_ListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UCM_ListEntry_String::OnToggleEditableState(const bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	CommonButton_PreviousOption->SetIsEnabled(bIsEditable);
	CommonRotator_AvailableOptions->SetIsEnabled(bIsEditable);
	CommonButton_NextOption->SetIsEnabled(bIsEditable);
}

void UCM_ListEntry_String::OnPreviousOptionButtonClicked() const
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}
	SelectThisEntryWidget();
}

void UCM_ListEntry_String::OnNextOptionButtonClicked() const
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}
	SelectThisEntryWidget();
}

void UCM_ListEntry_String::OnRotatorValueChanged(const int32 Value, const bool bUserInitiated) const
{
	if (!CachedOwningStringDataObject)
	{
		return;
	}

	const UCommonInputSubsystem* CommonInputSubsystem { GetInputSubsystem() };

	if (!CommonInputSubsystem || !bUserInitiated)
	{
		return;
	}

	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		CachedOwningStringDataObject->OnRotatorInitiatedValueChange(CommonRotator_AvailableOptions->GetSelectedText());
	}
}
