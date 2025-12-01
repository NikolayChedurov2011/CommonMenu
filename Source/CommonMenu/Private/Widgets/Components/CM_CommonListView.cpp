// ReSharper disable All
#include "Widgets/Components/CM_CommonListView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/CM_DataAsset_DataListEntryMapping.h"
#include "Widgets/Options/DataObjects/CM_ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/CM_ListDataObject_Collection.h"
#include "Widgets/Options/ListEntries/CM_ListEntry_Base.h"

#if WITH_EDITOR	
void UCM_CommonListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!DataListEntryMapping)
	{
		CompileLog.Error(FText::FromString(TEXT("The variable DataListEntryMapping has no valid data asset assigned.") +
			GetClass()->GetName() +
			TEXT(" needs a valid data asset to function properly.")));
	}
}
#endif

UUserWidget& UCM_CommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
	
	const TSubclassOf<UCM_ListEntry_Base> FoundWidgetClass { DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UCM_ListDataObject_Base>(Item)) };

	if (FoundWidgetClass)
	{
		return GenerateTypedEntry<UCM_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}
	else
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
}

bool UCM_CommonListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return !FirstSelectedItem->IsA<UCM_ListDataObject_Collection>();
}
