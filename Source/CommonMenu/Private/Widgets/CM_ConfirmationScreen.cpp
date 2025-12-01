// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CM_ConfirmationScreen.h"

#include "CommonTextBlock.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/CM_CommonButton_Base.h"


UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& ScreenTitle, const FText& ScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject {NewObject<UConfirmScreenInfoObject>()};
	InfoObject->CachedScreenTitle = ScreenTitle;
	InfoObject->CachedScreenMessage = ScreenMessage;

	FConfirmScreenButtonInfo OKButtonInfo{};
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OKButtonInfo.TextToDisplay = FText::FromString(TEXT("Ok"));

	InfoObject->AvailableScreenButtons.Add(OKButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& ScreenTitle,	const FText& ScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject {NewObject<UConfirmScreenInfoObject>()};
	InfoObject->CachedScreenTitle = ScreenTitle;
	InfoObject->CachedScreenMessage = ScreenMessage;

	FConfirmScreenButtonInfo YesButtonInfo{};
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.TextToDisplay = FText::FromString(TEXT("Yes"));

	FConfirmScreenButtonInfo NoButtonInfo{};
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.TextToDisplay = FText::FromString(TEXT("No"));

	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& ScreenTitle, const FText& ScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject {NewObject<UConfirmScreenInfoObject>()};
	InfoObject->CachedScreenTitle = ScreenTitle;
	InfoObject->CachedScreenMessage = ScreenMessage;

	FConfirmScreenButtonInfo OkButtonInfo{};
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.TextToDisplay = FText::FromString(TEXT("Ok"));

	FConfirmScreenButtonInfo CancelButtonInfo{};
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.TextToDisplay = FText::FromString(TEXT("Cancel"));

	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);
	
	return InfoObject;
}

void UCM_ConfirmationScreen::InitConfirmScreen(const UConfirmScreenInfoObject* ScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ButtonCallback)
{
	check(ScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);

	CommonTextBlock_Title->SetText(ScreenInfoObject->CachedScreenTitle);
	CommonTextBlock_Message->SetText(ScreenInfoObject->CachedScreenMessage);

	// Check of DynamicEntryBox_Buttons has old buttons created previously
	if (DynamicEntryBox_Buttons->GetNumEntries())
	{
		// The widget type for the entry box is specified in the child widget blueprint
		DynamicEntryBox_Buttons->Reset<UCM_CommonButton_Base>(
			[](const UCM_CommonButton_Base& Button)
			{
				Button.OnClicked().Clear();
			});
	}

	check(!ScreenInfoObject->AvailableScreenButtons.IsEmpty());

	for (const FConfirmScreenButtonInfo& ButtonInfo : ScreenInfoObject->AvailableScreenButtons)
	{
		UCM_CommonButton_Base* AddedButton {DynamicEntryBox_Buttons->CreateEntry<UCM_CommonButton_Base>()};
		AddedButton->SetButtonText(ButtonInfo.TextToDisplay);
		AddedButton->OnClicked().AddLambda([this, ButtonCallback, ButtonInfo]()
		{
			ButtonCallback(ButtonInfo.ConfirmScreenButtonType);

			DeactivateWidget();
		});
	}
}

UWidget* UCM_ConfirmationScreen::NativeGetDesiredFocusTarget() const
{
	if (DynamicEntryBox_Buttons->GetNumEntries())
	{
		DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
	}
	
	return Super::NativeGetDesiredFocusTarget();
}
