#include "Subsystems/CM_LoadingScreenSubsystem.h"

#include "PreLoadScreenManager.h"
#include "Settings/CM_LoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/CM_LoadingScreenInterface.h"

bool UCM_LoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		return false;
	}
	
	TArray<UClass*> FoundClasses{};
	GetDerivedClasses(GetClass(), FoundClasses);

	return FoundClasses.IsEmpty();
}

void UCM_LoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void UCM_LoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void UCM_LoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}

	SetTickableTickType(ETickableTickType::Conditional);

	bIsCurrentlyLoadingMap = true;

	TryUpdateLoadingScreen();
}

void UCM_LoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}
}

void UCM_LoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	if (IsPreLoadScreenActive())
	{
		return;
	}
	
	if (ShouldShowLoadingScreen())
	{
		TryDisplayLoadingScreenIfNone();
		
		OnLoadingReasonUpdate.Broadcast(CurrentLoadingReason);
	}
	else
	{
		TryRemoveLoadingScreen();

		HoldLoadingScreenStartUpTime = -1;
		
		NotifyLoadingScreenVisibilityChanged(false);
		
		SetTickableTickType(ETickableTickType::Never);
	}
}

void UCM_LoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
{
	if (CachedCreatedLoadingScreenWidget)
	{
		return;
	}
	
	const UCM_LoadingScreenSettings* LoadingScreenSettings { GetDefault<UCM_LoadingScreenSettings>() };

	const TSubclassOf<UUserWidget> LoadedWidgetClass { LoadingScreenSettings->GetLoadingScreenWidgetClassChecked() };

	UUserWidget* CreatedWidget { UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, NAME_None) };

	check(CreatedWidget);

	CachedCreatedLoadingScreenWidget = CreatedWidget->TakeWidget();
	
	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef(), 1000);

	NotifyLoadingScreenVisibilityChanged(true);
}

void UCM_LoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!CachedCreatedLoadingScreenWidget)
	{
		return;
	}

	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef());

	CachedCreatedLoadingScreenWidget.Reset();
}

void UCM_LoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(const bool IsVisible) const
{
	for (const ULocalPlayer* ExistingLocalPlayer : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLocalPlayer)
		{
			continue;
		}

		APlayerController* PlayerController { ExistingLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()) };

		if (PlayerController)
		{
			if (PlayerController->Implements<UCM_LoadingScreenInterface>())
			{
				if (IsVisible)
				{
					ICM_LoadingScreenInterface::Execute_OnLoadingScreenActivated(PlayerController);
				}
				else
				{
					ICM_LoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PlayerController);
				}
			}

			APawn* Pawn { PlayerController->GetPawn() };

			if (Pawn)
			{
				if (Pawn->Implements<UCM_LoadingScreenInterface>())
				{
					if (IsVisible)
					{
						ICM_LoadingScreenInterface::Execute_OnLoadingScreenActivated(Pawn);
					}
					else
					{
						ICM_LoadingScreenInterface::Execute_OnLoadingScreenDeactivated(Pawn);
					}
				}
			}
		}

		// The code for notifying other objects in the world goes here
	}
}

bool UCM_LoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	const FPreLoadScreenManager* PreLoadScreenManager { FPreLoadScreenManager::Get() };

	if (PreLoadScreenManager)
	{
		return PreLoadScreenManager->HasValidActivePreLoadScreen();
	}

	return false;
}

bool UCM_LoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UCM_LoadingScreenSettings* CM_LoadingScreenSettings { GetDefault<UCM_LoadingScreenSettings>() };

	if (GIsEditor && !CM_LoadingScreenSettings->bShouldLoadingScreenInEditor)
	{
		return false;
	}

	if (CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
		
		return true;
	}

	CurrentLoadingReason = TEXT("Waiting for Texture Streaming");

	// There's no need to show the loading screen. Allow to world to be rendered to our viewport here
	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	const double CurrentTime { FPlatformTime::Seconds() };

	if (HoldLoadingScreenStartUpTime < 0.)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}

	const double ElapsedTime { CurrentTime - HoldLoadingScreenStartUpTime };

	if (ElapsedTime < CM_LoadingScreenSettings->HoldLoadingScreenExtraSeconds)
	{
		return true;
	}
	
	return false;
}

bool UCM_LoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Level");
		return true;
	}

	const UWorld* World { GetGameInstance()->GetWorld() };

	if (!World)
	{
		CurrentLoadingReason = TEXT("Initializing World");
		return true;
	}

	if (!World->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World hasn't begun play yet");
		return true;
	}

	if (!World->GetFirstPlayerController())
	{
		CurrentLoadingReason = TEXT("Player Controller is not valid yet");
		return true;
	}

	return false;
	// Check if the game states, player states or player character actor component  are ready if needed
}

UWorld* UCM_LoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	const UGameInstance* GameInstance { GetGameInstance() };
	if (!GameInstance)
	{
		return nullptr;
	}
	return GameInstance->GetWorld();
}

void UCM_LoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType UCM_LoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}

	return ETickableTickType::Conditional;
}

bool UCM_LoadingScreenSubsystem::IsTickable() const
{
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UCM_LoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCM_LoadingScreenSubsystem, STATGROUP_Tickables);
}
