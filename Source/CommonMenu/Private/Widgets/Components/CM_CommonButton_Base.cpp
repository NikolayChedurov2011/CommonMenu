#include "Widgets/Components/CM_CommonButton_Base.h"

#include "CommonLazyImage.h"
#include "Subsystems/CM_UI_Subsystem.h"

#include "CommonTextBlock.h"

void UCM_CommonButton_Base::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}

void UCM_CommonButton_Base::SetButtonText(const FText NewButtonText) const
{
	if (!CommonTextBlock_ButtonText && NewButtonText.IsEmpty())
	{
		return;
	}

	CommonTextBlock_ButtonText->SetText(bUseUpperCaseForButtonText? NewButtonText.ToUpper() : NewButtonText);
}

FText UCM_CommonButton_Base::GetButtonDisplayText() const
{
	if (!CommonTextBlock_ButtonText)
	{
		return FText{};
	}

	return CommonTextBlock_ButtonText->GetText();
}

void UCM_CommonButton_Base::SetButtonDisplayImage(const FSlateBrush& Brush)
{
	if (CommonLazyImage_ButtonImage)
	{
		CommonLazyImage_ButtonImage->SetBrush(Brush);
	}
}

void UCM_CommonButton_Base::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (!CommonTextBlock_ButtonText && !GetCurrentTextStyleClass())
	{
		return;
	}

	CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
}

void UCM_CommonButton_Base::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (ButtonDescriptionText.IsEmpty())
	{
		return;
	}
	UCM_UI_Subsystem::Get(this)->OnDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
}

void UCM_CommonButton_Base::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	UCM_UI_Subsystem::Get(this)->OnDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
}
