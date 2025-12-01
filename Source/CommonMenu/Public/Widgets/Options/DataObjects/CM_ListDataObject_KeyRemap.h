#pragma once

#include "CoreMinimal.h"
#include "CM_ListDataObject_Base.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "CM_ListDataObject_KeyRemap.generated.h"

class UEnhancedInputUserSettings;
class UEnhancedPlayerMappableKeyProfile;

UCLASS()
class COMMONMENU_API UCM_ListDataObject_KeyRemap : public UCM_ListDataObject_Base
{
	GENERATED_BODY()

public:
	void InitKeyRemapData(UEnhancedInputUserSettings* InputUserSettings, UEnhancedPlayerMappableKeyProfile* KeyProfile, const ECommonInputType DesiredInputKeyType, const FPlayerKeyMapping& PlayerKeyMapping);

	FSlateBrush GetIconFromCurrentKey() const;

	void BindNewInputKey(const FKey& NewPressedKey);

	FORCEINLINE ECommonInputType GetDesiredInputKeyType() const { return CachedDesiredInputKeyType; }

	/*-- UCM_ListDataObject_Base --*/
	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	/*-- End UCM_ListDataObject_Base --*/
private:
	FPlayerKeyMapping* GetKeyMapping() const;
	
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedInputUserSettings{};
	
	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedKeyProfile{};

	ECommonInputType CachedDesiredInputKeyType{};

	FName CachedMappingName{};

	EPlayerMappableKeySlot CachedMappableKeySlot{};
};
