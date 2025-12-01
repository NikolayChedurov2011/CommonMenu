#include "Widgets/Options/CM_DataAsset_DataListEntryMapping.h"

#include "Widgets/Options/DataObjects/CM_ListDataObject_Base.h"

TSubclassOf<UCM_ListEntry_Base> UCM_DataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(const UCM_ListDataObject_Base* DataObject) const
{
	check(DataObject);
	
	for (UClass* DataObjectClass { DataObject->GetClass() } ; DataObjectClass ; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		TSubclassOf<UCM_ListDataObject_Base> ConvertedDataObjectClass { TSubclassOf<UCM_ListDataObject_Base>(DataObjectClass) };

		if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
		{
			return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
		}
	}

	return TSubclassOf<UCM_ListEntry_Base>();
}
