#include "Widgets/Components/CM_TabListWidget_Base.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Components/CM_CommonButton_Base.h"

void UCM_TabListWidget_Base::RequestRegisterTab(const FName& TabID, const FText& TabDisplayName)
{
	RegisterTab(TabID, TabButtonEntryWidgetClass, nullptr);

	UCM_CommonButton_Base* FoundButton {Cast<UCM_CommonButton_Base>(GetTabButtonBaseByID(TabID))};

	if (!FoundButton)
	{
		return;
	}

	FoundButton->SetButtonText(TabDisplayName);
}

#if WITH_EDITOR	
void UCM_TabListWidget_Base::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified.") +
			GetClass()->GetName() +
			TEXT(" needs a valid entry widget class.")));
	}
}
#endif

