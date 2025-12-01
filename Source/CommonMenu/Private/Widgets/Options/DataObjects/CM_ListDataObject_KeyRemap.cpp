#include "Widgets/Options/DataObjects/CM_ListDataObject_KeyRemap.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"

void UCM_ListDataObject_KeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* InputUserSettings, UEnhancedPlayerMappableKeyProfile* KeyProfile, const ECommonInputType DesiredInputKeyType,
                                                   const FPlayerKeyMapping& PlayerKeyMapping)
{
	CachedInputUserSettings = InputUserSettings;
	CachedKeyProfile = KeyProfile;
	CachedDesiredInputKeyType = DesiredInputKeyType;
	CachedMappingName = PlayerKeyMapping.GetMappingName();
	CachedMappableKeySlot = PlayerKeyMapping.GetSlot();
}

bool UCM_ListDataObject_KeyRemap::HasDefaultValue() const
{
	return GetKeyMapping()->GetDefaultKey().IsValid();
}

bool UCM_ListDataObject_KeyRemap::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && GetKeyMapping()->IsCustomized();
}

bool UCM_ListDataObject_KeyRemap::TryResetBackToDefaultValue()
{
	if (!CanResetBackToDefaultValue())
	{
		return false;
	}
	
	check(CachedInputUserSettings);
		
	GetKeyMapping()->ResetToDefault();

	CachedInputUserSettings->SaveSettings();

	NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

	return true;
}

FSlateBrush UCM_ListDataObject_KeyRemap::GetIconFromCurrentKey() const
{
	FSlateBrush FoundBrush{};

	UCommonInputSubsystem* CommonInputSubsystem { UCommonInputSubsystem::Get(CachedInputUserSettings->GetLocalPlayer()) };

	check(CommonInputSubsystem);
	
	const bool bHasFoundBrush { UCommonInputPlatformSettings::Get()->TryGetInputBrush(FoundBrush, GetKeyMapping()->GetCurrentKey(), CachedDesiredInputKeyType, CommonInputSubsystem->GetCurrentGamepadName()) };

	if (!bHasFoundBrush)
	{
		
	}
	
	return FoundBrush;
}


void UCM_ListDataObject_KeyRemap::BindNewInputKey(const FKey& NewPressedKey)
{
	check(CachedInputUserSettings);

	FMapPlayerKeyArgs KeyArgs{};
	KeyArgs.MappingName = CachedMappingName;
	KeyArgs.Slot = CachedMappableKeySlot;
	KeyArgs.NewKey = NewPressedKey;

	FGameplayTagContainer Container;
	CachedInputUserSettings->MapPlayerKey(KeyArgs, Container);
	CachedInputUserSettings->SaveSettings();

	NotifyListDataModified(this);
}

FPlayerKeyMapping* UCM_ListDataObject_KeyRemap::GetKeyMapping() const
{
	check(CachedKeyProfile);

	FMapPlayerKeyArgs KeyArgs{};
	KeyArgs.MappingName = CachedMappingName;
	KeyArgs.Slot = CachedMappableKeySlot;
	
	return CachedKeyProfile->FindKeyMapping(KeyArgs);
}
