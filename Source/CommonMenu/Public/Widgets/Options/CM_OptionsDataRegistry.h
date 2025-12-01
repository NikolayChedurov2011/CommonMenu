#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CM_OptionsDataRegistry.generated.h"

class UCM_ListDataObject_Base;
class UCM_ListDataObject_Collection;

UCLASS()
class COMMONMENU_API UCM_OptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	// Gets called by options screen right after the object of type  UCM_OptionsDataRegistry is created
	void InitOptionsDataRegistry(ULocalPlayer* OwningLocalPlayer);
	const TArray<UCM_ListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }
	TArray<UCM_ListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& SelectedTabID) const;
	
private:
	void FindChildListDataRecursively(const UCM_ListDataObject_Base* ParentData, TArray<UCM_ListDataObject_Base*>& OutFountChildListData) const;
	
	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab(ULocalPlayer* OwningLocalPlayer);

	UPROPERTY(Transient)
	TArray<UCM_ListDataObject_Collection*> RegisteredOptionsTabCollections{};
};
