#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CM_GameUserSettings.generated.h"

UCLASS()
class COMMONMENU_API UCM_GameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UCM_GameUserSettings();
	
	static UCM_GameUserSettings* Get();

	/*-- Gameplay Collection Tab --*/
	UFUNCTION()
	FString GetCurrentDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentDifficulty(const FString& NewGameDifficulty) { CurrentGameDifficulty = NewGameDifficulty; }
	/*-- End Gameplay Collection Tab --*/

	/*-- Audio Collection Tab --*/
	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; }

	UFUNCTION()
	void SetOverallVolume(const float NewOverallVolume);

	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION()
	void SetMusicVolume(const float NewMusicVolume);

	UFUNCTION()
	float GetSoundFXVolume() const { return SoundFXVolume; }

	UFUNCTION()
	void SetSoundFXVolume(const float NewSoundFXVolume);

	UFUNCTION()
	bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }

	UFUNCTION()
	void SetAllowBackgroundAudio(const bool NewAllowBackgroundAudio);

	UFUNCTION()
	bool GetUseHDRAudioMode() const { return bUseHDRAudioMode; }

	UFUNCTION()
	void SetUseHDRAudioMode(const bool NewUseHDRAudioMode);
	/*-- End Audio Collection Tab --*/

	/*-- Video Collection Tab --*/
	UFUNCTION()
	float GetCurrentDisplayGamma() const;

	UFUNCTION()
	void SetCurrentDisplayGamma(const float NewGamma);
	/*-- End Video Collection Tab --*/
	
private:
	/*-- Gameplay Collection Tab --*/
	UPROPERTY(Config)
	FString CurrentGameDifficulty{};
	/*-- End Gameplay Collection Tab --*/

	/*-- Audio Collection Tab --*/
	UPROPERTY(Config)
	float OverallVolume{};

	UPROPERTY(Config)
	float MusicVolume{};

	UPROPERTY(Config)
	float SoundFXVolume{};

	UPROPERTY(Config)
	bool bAllowBackgroundAudio{};

	UPROPERTY(Config)
	bool bUseHDRAudioMode{};
	/*-- End Audio Collection Tab --*/

	/*-- Video Collection Tab --*/
	/*-- End Video Collection Tab --*/
};
