#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CM_DataAsset_DataListEntryMapping.generated.h"

class UCM_ListEntry_Base;
class UCM_ListDataObject_Base;

UCLASS()
class COMMONMENU_API UCM_DataAsset_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UCM_ListEntry_Base> FindEntryWidgetClassByDataObject(const UCM_ListDataObject_Base* DataObject) const;
	
private:

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UCM_ListDataObject_Base>, TSubclassOf<UCM_ListEntry_Base>> DataObjectListEntryMap{};
};
