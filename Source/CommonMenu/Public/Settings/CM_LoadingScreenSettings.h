#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CM_LoadingScreenSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta=(DisplayName="Common Menu Loading Screen Settings"))
class COMMONMENU_API UCM_LoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;
	
	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass{};

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	float HoldLoadingScreenExtraSeconds {3.f};

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	bool bShouldLoadingScreenInEditor {false};
};
