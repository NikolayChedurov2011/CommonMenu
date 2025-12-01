#include "AsyncActions/CM_AsyncAction_PushSoftWidget.h"

#include "Subsystems/CM_UI_Subsystem.h"
#include "Widgets/CM_ActivableWidget_Base.h"

void UCM_AsyncAction_PushSoftWidget::Activate()
{
	UCM_UI_Subsystem* CM_UI_Subsystem = UCM_UI_Subsystem::Get(CachedWorld.Get());

	CM_UI_Subsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedSoftWidgetClass,
		[this](const EAsyncPushWidgetState PushWidgetState, UCM_ActivableWidget_Base* PushedActivableWidget)
	{
		switch (PushWidgetState)
		{
			case EAsyncPushWidgetState::OnCreatedBeforePush :

				PushedActivableWidget->SetOwningPlayer(CachedPlayerController.Get());
				OnWidgetCreatedBeforePush.Broadcast(PushedActivableWidget);
				break;
			
			case EAsyncPushWidgetState::AfterPush :

				AfterPush.Broadcast(PushedActivableWidget);

				if (bCachedFocusOnCreatedWidget)
				{
					if (UWidget* WidgetToFocus {PushedActivableWidget->GetDesiredFocusTarget()})
					{
						WidgetToFocus->SetFocus();
					}
				}
				SetReadyToDestroy();
				break;

			default :
				break;
		}
	});
}

UCM_AsyncAction_PushSoftWidget* UCM_AsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject,
                                                                               APlayerController* OwningPlayerController, TSoftClassPtr<UCM_ActivableWidget_Base> SoftWidgetClass, UPARAM(meta=(Categories = "UI.WidgetStack")) const FGameplayTag WidgetStackTag, bool bFocusOnCreatedWidget)
{
	checkf(!SoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToWidgetStack was passed with a null soft widget class"));

	if (!GEngine)
	{
		return nullptr;
	}

	UWorld* World{GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)};
	if (!World)
	{
		return nullptr;
	}

	UCM_AsyncAction_PushSoftWidget* Node {NewObject<UCM_AsyncAction_PushSoftWidget>()};
	Node->CachedWorld = World;
	Node->CachedPlayerController = OwningPlayerController;
	Node->CachedSoftWidgetClass = SoftWidgetClass;
	Node->CachedWidgetStackTag = WidgetStackTag;
	Node->bCachedFocusOnCreatedWidget = bFocusOnCreatedWidget;
	
	Node->RegisterWithGameInstance(World);

	return Node;
}
