#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CM_LoadingScreenSubsystem.generated.h"

UCLASS()
class COMMONMENU_API UCM_LoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdateDelegate, const FString&, CurrentLoadingScreenReason);

	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdateDelegate OnLoadingReasonUpdate;
	
	/*-- USubsystem --*/
	/** Override to control if the Subsystem should be created at all.
	 * For example you could only have your system created on servers.
	 * It's important to note that if using this is becomes very important to null check whenever getting the Subsystem.
	 *
	 * Note: This function is called on the CDO prior to instances being created!
	 *
	 */
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize();
	/*-- End USubsystem --*/

	/*-- FTickableGameObject --*/
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	/*-- End FTickableGameObject --*/

private:
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);
	void OnMapPostLoaded(UWorld* LoadedWorld);

	void TryUpdateLoadingScreen();
	void TryDisplayLoadingScreenIfNone();
	void TryRemoveLoadingScreen();
	
	void NotifyLoadingScreenVisibilityChanged(const bool IsVisible) const;

	bool IsPreLoadScreenActive() const;
	bool ShouldShowLoadingScreen();
	bool CheckTheNeedToShowLoadingScreen();

	bool bIsCurrentlyLoadingMap {false};
	double HoldLoadingScreenStartUpTime { -1.f };
	FString CurrentLoadingReason{};

	TSharedPtr<SWidget> CachedCreatedLoadingScreenWidget{};
};
