#include "Widgets/Options/DataObjects/CM_ListDataObject_Collection.h"

void UCM_ListDataObject_Collection::AddChildListData(UCM_ListDataObject_Base* ChildListData)
{
	ChildListData->InitDataObject();
	ChildListData->SetParentData(this);
	ChildListDataArray.Add(ChildListData);
}

TArray<UCM_ListDataObject_Base*> UCM_ListDataObject_Collection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool UCM_ListDataObject_Collection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}
