#include "Settings/CM_LoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UCM_LoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("Forgot to assign a valid widget blueprint in the project settings as loading screen"));

	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget { SoftLoadingScreenWidgetClass.LoadSynchronous() };

	return LoadedLoadingScreenWidget;
}
