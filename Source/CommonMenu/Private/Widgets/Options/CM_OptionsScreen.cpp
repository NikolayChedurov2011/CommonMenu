#include "Widgets/Options/CM_OptionsScreen.h"

#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"
#include "Settings/CM_GameUserSettings.h"
#include "Subsystems/CM_UI_Subsystem.h"
#include "Widgets/Components/CM_CommonButton_Base.h"
#include "Widgets/Components/CM_CommonListView.h"
#include "Widgets/Components/CM_TabListWidget_Base.h"
#include "Widgets/Options/DataObjects/CM_ListDataObject_Collection.h"
#include "Widgets/Options/CM_OptionsDataRegistry.h"
#include "Widgets/Options/CM_OptionsDetailsView.h"
#include "Widgets/Options/ListEntries/CM_ListEntry_Base.h"

void UCM_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(FBindUIActionArgs{
														ResetAction,
														true,
														FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)});
	}
	
	RegisterUIActionBinding(FBindUIActionArgs{
								ICommonInputModule::GetSettings().GetDefaultBackAction(),
								true,
								FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)});

	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);

	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
}

void UCM_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (const UCM_ListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
		{
			continue;
		}

		const FName TabID {TabCollection->GetDataID()};
		const FText TabDisplayName {TabCollection->GetDataDisplayName()};
		
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID))
		{
			continue;
		}
		
		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabDisplayName);
	}
}

void UCM_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UCM_GameUserSettings::Get()->ApplySettings(true);
}

UWidget* UCM_OptionsScreen::NativeGetDesiredFocusTarget() const
{
	const UObject* SelectedObject { CommonListView_OptionsList->GetSelectedItem() };

	if (!SelectedObject)
	{
		return nullptr;
	}

	UUserWidget* SelectedItem { CommonListView_OptionsList->GetEntryWidgetFromItem(SelectedObject) };

	if (!SelectedItem)
	{
		return nullptr;
	}
	
	return SelectedItem;
}

void UCM_OptionsScreen::OnOptionsTabSelected(const FName TabID)
{
	DetailsView_ListEntryInfo->ClearDetailsViewInfo();
	
	const TArray<UCM_ListDataObject_Base*> FoundListSourceItems { GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabID) };

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems())
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();

	for (UCM_ListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewListDataModified);
		}
		
		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UCM_OptionsScreen::OnListViewItemHovered(UObject* HoveredItem, const bool bWasHovered) const
{
	if (!HoveredItem)
	{
		return;
	}

	UCM_ListEntry_Base* HoveredEntryWidget { CommonListView_OptionsList->GetEntryWidgetFromItem<UCM_ListEntry_Base>(HoveredItem) };

	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);

	if (bWasHovered)
	{
		DetailsView_ListEntryInfo->UpdateDetailsViewInfo(CastChecked<UCM_ListDataObject_Base>(HoveredItem), TryGteEntryWidgetClassName(HoveredItem));
	}
	else
	{
		UCM_ListDataObject_Base* SelectedItem { CommonListView_OptionsList->GetSelectedItem<UCM_ListDataObject_Base>() };

		if (SelectedItem)
		{
			DetailsView_ListEntryInfo->UpdateDetailsViewInfo(SelectedItem, TryGteEntryWidgetClassName(SelectedItem));
		}
	}
}

void UCM_OptionsScreen::OnListViewItemSelected(UObject* SelectedItem) const
{
	if (!SelectedItem)
	{
		return;
	}

	DetailsView_ListEntryInfo->UpdateDetailsViewInfo(CastChecked<UCM_ListDataObject_Base>(SelectedItem), TryGteEntryWidgetClassName(SelectedItem));
}

void UCM_OptionsScreen::OnResetBoundActionTriggered()
{
	if (ResettableDataArray.IsEmpty())
	{
		return;
	}

	UCommonButtonBase* SelectedButton { TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab()) };
	const FString SelectedTabButtonName { CastChecked<UCM_CommonButton_Base>(SelectedButton)->GetButtonDisplayText().ToString() };
	
	UCM_UI_Subsystem::Get(this)->PushConfirmScreenToModalStackAsync(EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset")),
		FText::FromString(TEXT("Are you sure you want to reset all the settings under the ") + SelectedTabButtonName + TEXT(" tab.")),
		[this](const EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}

			bIsResettingData = true;
			bool bSuccessfulReset {false};
			
			for (UCM_ListDataObject_Base* DataToReset : ResettableDataArray)
			{
				if (!DataToReset)
				{
					continue;
				}

				bSuccessfulReset = DataToReset->TryResetBackToDefaultValue();
			}

			if (bSuccessfulReset)
			{
				ResettableDataArray.Empty();
				RemoveActionBinding(ResetActionHandle);
			}

			bIsResettingData = false;
		});
}

void UCM_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

UCM_OptionsDataRegistry* UCM_OptionsScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UCM_OptionsDataRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	checkf(CreatedOwningDataRegistry, TEXT("Data registry for options screen is not valid"));

	return CreatedOwningDataRegistry;
}

FString UCM_OptionsScreen::TryGteEntryWidgetClassName(const UObject* OwningListItem) const
{
	UUserWidget* FoundEntryWidget { CommonListView_OptionsList->GetEntryWidgetFromItem(OwningListItem) };

	if (!FoundEntryWidget)
	{
		return FString{};
	}

	return FoundEntryWidget->GetClass()->GetName();
}

void UCM_OptionsScreen::OnListViewListDataModified(UCM_ListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (!ModifiedData || bIsResettingData)
	{
		return;
	}

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}
