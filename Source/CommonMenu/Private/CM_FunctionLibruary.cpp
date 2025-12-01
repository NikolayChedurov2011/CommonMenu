#include "CM_FunctionLibruary.h"

#include "Settings/CM_DeveloperSettings.h"

TSoftClassPtr<UCM_ActivableWidget_Base> UCM_FunctionLibruary::GetSoftWidgetClassByTag(UPARAM(meta=(Categories = "UI.Widget")) const FGameplayTag WidgetTag)
{
	const UCM_DeveloperSettings* CM_DeveloperSettings{GetDefault<UCM_DeveloperSettings>()};

	checkf(CM_DeveloperSettings->WidgetMap.Contains(WidgetTag), TEXT("Could not find the widget under the tag %s"), *WidgetTag.ToString());
	
	return CM_DeveloperSettings->WidgetMap.FindRef(WidgetTag);
}

TSoftObjectPtr<UTexture2D> UCM_FunctionLibruary::GetOptionsSoftImageByTag(UPARAM(meta=(Categories = "UI.Image")) const FGameplayTag ImageTag)
{
	const UCM_DeveloperSettings* CM_DeveloperSettings{GetDefault<UCM_DeveloperSettings>()};

	checkf(CM_DeveloperSettings->OptionsScreenSoftImageMap.Contains(ImageTag), TEXT("Could not find the soft image under the tag %s"), *ImageTag.ToString());
	
	return CM_DeveloperSettings->OptionsScreenSoftImageMap.FindRef(ImageTag);
}