#include "Widgets/Components/CM_CommonRotator.h"

#include "CommonTextBlock.h"

void UCM_CommonRotator::SetSelectedOptionByText(const FText& OptionText)
{
	const int32 FoundIndex = TextLabels.IndexOfByPredicate([OptionText](const FText& TextItem)->bool
	{
		return TextItem.EqualTo(OptionText);
	});

	if (FoundIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundIndex);
	}
	else
	{
		MyText->SetText(OptionText);
	}
}
