#include "Widgets/Options/DataObjects/CM_ListDataObject_Value.h"

void UCM_ListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FCM_OptionsDataInteractionHelper>& DynamicGetter)
{
	DataDynamicGetter = DynamicGetter;
}

void UCM_ListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FCM_OptionsDataInteractionHelper>& DynamicSetter)
{
	DataDynamicSetter = DynamicSetter;
}
