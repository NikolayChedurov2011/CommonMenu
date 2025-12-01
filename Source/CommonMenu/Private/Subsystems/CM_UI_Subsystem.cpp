#include "Subsystems/CM_UI_Subsystem.h"

#include "CM_FunctionLibruary.h"
#include "Widgets/CM_Layout_Base.h"
#include "Widgets/CM_ActivableWidget_Base.h"

#include "Engine/AssetManager.h"
#include "Tags/CM_GameplayTags.h"
#include "Widgets/CM_ConfirmationScreen.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UCM_UI_Subsystem* UCM_UI_Subsystem::Get(const UObject* WorldContextObject)
{
	if (!GEngine)
	{
		return nullptr;
	}

	const UWorld* World{GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)};
	
	return UGameInstance::GetSubsystem<UCM_UI_Subsystem>(World->GetGameInstance());
}

bool UCM_UI_Subsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		return false;
	}
	
	TArray<UClass*> FoundClasses{};
	GetDerivedClasses(GetClass(), FoundClasses);

	return FoundClasses.IsEmpty();
}

void UCM_UI_Subsystem::RegisterCreatedPrimaryLayoutWidget(UCM_Layout_Base* Layout)
{
	check(Layout);
	RegisteredLayout = Layout;
}

void UCM_UI_Subsystem::PushSoftWidgetToStackAsync(const FGameplayTag& WidgetStackTag,	TSoftClassPtr<UCM_ActivableWidget_Base> SoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UCM_ActivableWidget_Base*)> AsyncPushStateCallback) const
{
	check(!SoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(SoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this, SoftWidgetClass, WidgetStackTag, AsyncPushStateCallback]()
		{
			UClass* LoadedWidgetClass {SoftWidgetClass.Get()};
			check(LoadedWidgetClass && RegisteredLayout);
			
			UCommonActivatableWidgetContainerBase* FoundWidgetStack {RegisteredLayout->FindWidgetStackByTag(WidgetStackTag)};

			UCM_ActivableWidget_Base* CreatedWidget = FoundWidgetStack->AddWidget<UCM_ActivableWidget_Base>(LoadedWidgetClass, [AsyncPushStateCallback](UCM_ActivableWidget_Base& CreatedWidgetInstance)
			{
				AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
			});

			AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
		}));
}

void UCM_UI_Subsystem::PushConfirmScreenToModalStackAsync(const EConfirmScreenType ScreenType, const FText& ScreenTitle,	const FText& ScreenMessage,	TFunction<void(EConfirmScreenButtonType)> ButtonCallback) const
{
	UConfirmScreenInfoObject* CreatedInfoObject{};
		
	switch (ScreenType)
	{
		case EConfirmScreenType::Ok :
			CreatedInfoObject = UConfirmScreenInfoObject::CreateOKScreen(ScreenTitle, ScreenMessage);
			break;

		case EConfirmScreenType::YesNo :
			CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(ScreenTitle, ScreenMessage);
			break;

		case EConfirmScreenType::OkCancel :
			CreatedInfoObject = UConfirmScreenInfoObject::CreateOkCancelScreen(ScreenTitle, ScreenMessage);	
			break;

		case EConfirmScreenType::Unknown :
			
			break;

		default :
			break;
	}

	check(CreatedInfoObject);

	PushSoftWidgetToStackAsync(CMGameplayTags::UI_WidgetStack_Modal, UCM_FunctionLibruary::GetSoftWidgetClassByTag(CMGameplayTags::UI_Widget_ConfirmScreen),
		[CreatedInfoObject, ButtonCallback](const EAsyncPushWidgetState State, UCM_ActivableWidget_Base* PushedWidget)
		{
			if (State == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UCM_ConfirmationScreen* CreatedConfirmScreen{CastChecked<UCM_ConfirmationScreen>(PushedWidget)};
				CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonCallback);
			}
		});
}
