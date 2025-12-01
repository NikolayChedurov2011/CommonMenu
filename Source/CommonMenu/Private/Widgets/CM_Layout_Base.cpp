#include "Widgets/CM_Layout_Base.h"

UCommonActivatableWidgetContainerBase* UCM_Layout_Base::FindWidgetStackByTag(const FGameplayTag StackTag) const
{
	checkf(RegisteredWidgetsStackMap.Contains(StackTag), TEXT("Can't find the widget stack by the tag %s"), *StackTag.ToString());
	
	return *RegisteredWidgetsStackMap.Find(StackTag);
}

void UCM_Layout_Base::RegisterWidgetStack(UPARAM(meta=(Categories = "UI.WidgetStack")) const FGameplayTag WidgetStackTag, UCommonActivatableWidgetContainerBase* Stack)
{
	if (IsDesignTime())
	{
		return;
	}

	if (RegisteredWidgetsStackMap.Contains(WidgetStackTag))
	{
		return;
	}

	RegisteredWidgetsStackMap.Add(WidgetStackTag, Stack);
}
