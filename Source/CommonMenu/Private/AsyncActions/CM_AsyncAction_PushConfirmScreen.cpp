#include "AsyncActions/CM_AsyncAction_PushConfirmScreen.h"

#include "Subsystems/CM_UI_Subsystem.h"

void UCM_AsyncAction_PushConfirmScreen::Activate()
{
	UCM_UI_Subsystem* CM_UI_Subsystem = UCM_UI_Subsystem::Get(CachedWorld.Get());

	CM_UI_Subsystem->PushConfirmScreenToModalStackAsync(CachedConfirmScreenType, CachedScreenTitle, CachedScreenMessage, 
		[this](const EConfirmScreenButtonType ButtonType)
			{
				OnButtonClicked.Broadcast(ButtonType);

				SetReadyToDestroy();
			});
}

UCM_AsyncAction_PushConfirmScreen* UCM_AsyncAction_PushConfirmScreen::PushConfirmScreen(const UObject* WorldContextObject, EConfirmScreenType ScreenType, FText ScreenTitle, FText ScreenMessage)
{
	if (!GEngine)
	{
		return nullptr;
	}

	UWorld* World{GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)};
	if (!World)
	{
		return nullptr;
	}

	UCM_AsyncAction_PushConfirmScreen* Node {NewObject<UCM_AsyncAction_PushConfirmScreen>()};
	Node->CachedWorld = World;
	Node->CachedConfirmScreenType = ScreenType;
	Node->CachedScreenTitle = ScreenTitle;
	Node->CachedScreenMessage = ScreenMessage;

	Node->RegisterWithGameInstance(World);

	return Node;
}
