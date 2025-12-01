#include "Settings/CM_GameUserSettings.h"

#include "Kismet/GameplayStatics.h"
#include "Settings/CM_DeveloperSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

UCM_GameUserSettings::UCM_GameUserSettings() : OverallVolume(1.f), MusicVolume(1.f), SoundFXVolume(1.f), bAllowBackgroundAudio(false), bUseHDRAudioMode(false)
{
	
}

UCM_GameUserSettings* UCM_GameUserSettings::Get()
{
	if (!GEngine)
	{
		return nullptr;
	}

	return CastChecked<UCM_GameUserSettings>(GEngine->GetGameUserSettings());
}

/*-- Audio Collection Tab --*/
void UCM_GameUserSettings::SetOverallVolume(const float NewOverallVolume)
{
	OverallVolume = NewOverallVolume;

	// The actual logic for controlling the volume goes here

	const UWorld* AudioWorld = nullptr;
	const UCM_DeveloperSettings* FrontendDeveloperSettings = GetDefault<UCM_DeveloperSettings>();
 
	if (GEngine)
	{
		AudioWorld = GEngine->GetCurrentPlayWorld();
	}
 
	if (!AudioWorld || !FrontendDeveloperSettings)
	{
		return;
	}
 
	USoundClass* MasterSoundClass = nullptr;
	if (UObject* LoadedObject = FrontendDeveloperSettings->MasterSoundClass.TryLoad())
	{
		MasterSoundClass = CastChecked<USoundClass>(LoadedObject);
	}
 
	USoundMix* DefaultSoundMix = nullptr;
	if (UObject* LoadedObject = FrontendDeveloperSettings->DefaultSoundMix.TryLoad())
	{
		DefaultSoundMix = CastChecked<USoundMix>(LoadedObject);
	}
 
	OverallVolume = NewOverallVolume;
 
	UGameplayStatics::SetSoundMixClassOverride(
		AudioWorld,
		DefaultSoundMix,
		MasterSoundClass,
		OverallVolume,
		1.f,
		0.2f
	);
 
	UGameplayStatics::PushSoundMixModifier(AudioWorld, DefaultSoundMix);
}

void UCM_GameUserSettings::SetMusicVolume(const float NewMusicVolume)
{
	MusicVolume = NewMusicVolume;

	// The actual logic for controlling the volume goes here

}

void UCM_GameUserSettings::SetSoundFXVolume(const float NewSoundFXVolume)
{
	SoundFXVolume = NewSoundFXVolume;

	// The actual logic for controlling the volume goes here

}

void UCM_GameUserSettings::SetAllowBackgroundAudio(const bool NewAllowBackgroundAudio)
{
	bAllowBackgroundAudio = NewAllowBackgroundAudio;

	// The actual logic for controlling the volume goes here
	
}

void UCM_GameUserSettings::SetUseHDRAudioMode(const bool NewUseHDRAudioMode)
{
	bUseHDRAudioMode = NewUseHDRAudioMode;

	// The actual logic for controlling the volume goes here
	
}
/*-- End Audio Collection Tab --*/

/*-- Video Collection Tab --*/
float UCM_GameUserSettings::GetCurrentDisplayGamma() const
{
	if (!GEngine)
	{
		return 0.f;
	}

	return GEngine->GetDisplayGamma();
}

void UCM_GameUserSettings::SetCurrentDisplayGamma(const float NewGamma)
{
	if (!GEngine)
	{
		return;
	}

	GEngine->DisplayGamma = NewGamma;
}
/*-- End Video Collection Tab --*/