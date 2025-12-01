#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "CM_DeveloperSettings.generated.h"

class UCM_ActivableWidget_Base;

UCLASS(Config = Game, DefaultConfig, meta=(DisplayName="Common Menu UI Settings"))
class COMMONMENU_API UCM_DeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta=(ForceInLineRow, Categories="UI.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UCM_ActivableWidget_Base>> WidgetMap;

	UPROPERTY(Config, EditAnywhere, Category = "Options Image Reference", meta=(ForceInLineRow, Categories="UI.Image"))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> OptionsScreenSoftImageMap;

	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MasterSoundClass;
 
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath MusicSoundClass;
 
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
	FSoftObjectPath SoundFXSoundClass;
 
	UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundMix"))
	FSoftObjectPath DefaultSoundMix;
};
