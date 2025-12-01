#include "Widgets/Options/ListEntries/CM_ListEntry_KeyRemap.h"

#include "CM_FunctionLibruary.h"
#include "Subsystems/CM_UI_Subsystem.h"
#include "Tags/CM_GameplayTags.h"
#include "Widgets/Components/CM_CommonButton_Base.h"
#include "Widgets/Options/CM_KeyRemapScreen.h"
#include "Widgets/Options/DataObjects/CM_ListDataObject_KeyRemap.h"

void UCM_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UCM_ListEntry_KeyRemap::OnOwningListDataObjectSet(UCM_ListDataObject_Base* OwningListDataObject)
{
	Super::OnOwningListDataObjectSet(OwningListDataObject);

	CachedKeyRemapDataObject = CastChecked<UCM_ListDataObject_KeyRemap>(OwningListDataObject);

	CommonButton_RemapKey->SetButtonDisplayImage(CachedKeyRemapDataObject->GetIconFromCurrentKey());
}

void UCM_ListEntry_KeyRemap::OnOwningListDataObjectModified(UCM_ListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedKeyRemapDataObject->GetIconFromCurrentKey());
	}
}

void UCM_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();
	
	UCM_UI_Subsystem::Get(this)->PushSoftWidgetToStackAsync(CMGameplayTags::UI_WidgetStack_Modal,
		UCM_FunctionLibruary::GetSoftWidgetClassByTag(CMGameplayTags::UI_Widget_KeyRemapScreen),
		[this](const EAsyncPushWidgetState PushState, UCM_ActivableWidget_Base* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UCM_KeyRemapScreen* CreatedKeyRemapScreen { CastChecked<UCM_KeyRemapScreen>(PushedWidget) };
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);
				
				if (CachedKeyRemapDataObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedKeyRemapDataObject->GetDesiredInputKeyType());
				}
			}
		});
}

void UCM_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!CachedKeyRemapDataObject)
	{
		return;
	}

	// Check if the current key is already the default key. Display an OK screen that says this is already the default key to the player
	if (!CachedKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UCM_UI_Subsystem::Get(this)->PushConfirmScreenToModalStackAsync(EConfirmScreenType::Ok,
			FText::FromString(TEXT("Reset Key Mapping")),
			FText::FromString(TEXT("The Key binding for ") + CachedKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" is already set to default.")),
			[](EConfirmScreenButtonType ClickedButton)
			{
				
			});
		return;
	}

	UCM_UI_Subsystem::Get(this)->PushConfirmScreenToModalStackAsync(EConfirmScreenType::YesNo,
			FText::FromString(TEXT("Reset Key Mapping")),
			FText::FromString(TEXT("Are you sure you want to reset the key binding for ") + CachedKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT("?")),
			[this](EConfirmScreenButtonType ClickedButton)
			{
				if (ClickedButton == EConfirmScreenButtonType::Confirmed)
				{
					CachedKeyRemapDataObject->TryResetBackToDefaultValue();
				}
			});
	// Resert 
}

void UCM_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedKeyRemapDataObject)
	{
		CachedKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UCM_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UCM_UI_Subsystem::Get(this)->PushConfirmScreenToModalStackAsync(EConfirmScreenType::Ok, FText::FromString(TEXT("Key Remap")), FText::FromString(CanceledReason),
		[](const EConfirmScreenButtonType ClickedButton)
		{
			
		});	
}
