#include "Widgets/Options/CM_OptionsDetailsView.h"

#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Widgets/Options/DataObjects/CM_ListDataObject_Base.h"

void UCM_OptionsDetailsView::UpdateDetailsViewInfo(UCM_ListDataObject_Base* DataObject, const FString& EntryWidgetClassName)
{
	if (!DataObject)
	{
		return;
	}

	CommonTextBlock_Title->SetText(DataObject->GetDataDisplayName());

	if (!DataObject->GetSoftDescriptionImage().IsNull())
	{
		CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(DataObject->GetSoftDescriptionImage());
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	CommonRichTextBlock_Description->SetText(DataObject->GetDescriptionRichText());

	const FString DynamicDetails { FString::Printf(
		TEXT("Data Object Class: <Bold>%s</>\n\nEntry Widget Class: <Bold>%s</>"), *DataObject->GetClass()->GetName(), *EntryWidgetClassName
	) };
	CommonRichTextBlock_DynamicDetails->SetText(FText::FromString(DynamicDetails));
	
	CommonRichTextBlock_DisabledReason->SetText(DataObject->IsDataCurrentlyEditable()? FText::GetEmpty() : DataObject->GetDisabledRichText());
}

void UCM_OptionsDetailsView::ClearDetailsViewInfo() const
{
	CommonTextBlock_Title->SetText(FText::GetEmpty());
	CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	CommonRichTextBlock_Description->SetText(FText::GetEmpty());
	CommonRichTextBlock_DynamicDetails->SetText(FText::GetEmpty());
	CommonRichTextBlock_DisabledReason->SetText(FText::GetEmpty());
}

void UCM_OptionsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetailsViewInfo();
}
