#include "Widgets/Options/CM_KeyRemapScreen.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "CommonRichTextBlock.h"
#include "CommonUITypes.h"
#include "ICommonInputModule.h"
#include "Framework/Application/IInputProcessor.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputPreprocessor(const ECommonInputType InputTypeToListenTo, ULocalPlayer* LocalPlayer) : CachedInputTypeToListenTo(InputTypeToListenTo), CachedWeakLocalPlayer(LocalPlayer)
	{ }

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDelegate, const FKey& /*PressedKey*/);
	FOnInputPreProcessorKeyPressedDelegate OnInputPreProcessorKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectedCanceledDelegate, const FString& /*CanceledReason*/);
	FOnInputPreProcessorKeySelectedCanceledDelegate OnInputPreProcessorKeySelectedCanceled;


protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());

		return true;
	}
	
	virtual bool HandleMouseButtonDownEvent( FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());

		return true;
	}

	void ProcessPressedKey(const FKey& PressedKey)
	{
		if (PressedKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Key Remap has been canceled"));

			return;
		}

		UCommonInputSubsystem* CommonInputSubsystem { UCommonInputSubsystem::Get(CachedWeakLocalPlayer.Get()) };

		check(CommonInputSubsystem);

		const ECommonInputType CurrentInputType { CommonInputSubsystem->GetCurrentInputType() };

		switch (CachedInputTypeToListenTo)
		{
			case ECommonInputType::MouseAndKeyboard :
				if (PressedKey.IsGamepadKey() || CurrentInputType == ECommonInputType::Gamepad)
				{
					OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Detected Gamepad Key pressed for Keyboard inputs. Key Remap has been canceled"));
					return;
				}
				break;
				
			case ECommonInputType::Gamepad :
				if (CurrentInputType == ECommonInputType::Gamepad && PressedKey == EKeys::LeftMouseButton)
				{
					FCommonInputActionDataBase* InputActionData { ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT("")) };

					check(InputActionData);
					
					OnInputPreProcessorKeyPressed.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());

					return;
				}
			
				if (!PressedKey.IsGamepadKey())
				{
					OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Detected non Gamepad Key pressed for Gamepad inputs. Key Remap has been canceled"));
					return;
				}
				break;
			
			default :			
				break;
		}
		OnInputPreProcessorKeyPressed.ExecuteIfBound(PressedKey);
	}

private:
	ECommonInputType CachedInputTypeToListenTo{};
	TWeakObjectPtr<ULocalPlayer> CachedWeakLocalPlayer{};
};

void UCM_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(CachedDesiredInputType, GetOwningLocalPlayer());
	CachedInputPreprocessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressedDetected);
	CachedInputPreprocessor->OnInputPreProcessorKeySelectedCanceled.BindUObject(this, &ThisClass::OnKeySelectCancelled);

	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1);

	FString InputDeviceName{};

	switch (CachedDesiredInputType)
	{
		case ECommonInputType::MouseAndKeyboard :
			InputDeviceName = TEXT("Mouse & Keyboard");
		
			break;
					
		case ECommonInputType::Gamepad :
			InputDeviceName = TEXT("Gamepad");
		
			break;
				
		default :			
			break;
	}

	const FString DisplayRichMessage { FString::Printf(TEXT("<KeyRemapDefault>Press any </><KeyRemapHighlight>%s</><KeyRemapDefault> key.</>"), *InputDeviceName) };

	CommonRichText_RemapMessage->SetText(FText::FromString(DisplayRichMessage));
}

void UCM_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreprocessor);
		CachedInputPreprocessor.Reset();
	}
}

void UCM_KeyRemapScreen::SetDesiredInputTypeToFilter(const ECommonInputType DesiredInputType)
{
	CachedDesiredInputType = DesiredInputType;
}

void UCM_KeyRemapScreen::OnValidKeyPressedDetected(const FKey& PressedKey)
{
	RequestDeactivateWidget([this, PressedKey]()
	{
		OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
	});
}

void UCM_KeyRemapScreen::OnKeySelectCancelled(const FString& CanceledReason)
{
	RequestDeactivateWidget([this, CanceledReason]()
	{
		OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
	});
}

void UCM_KeyRemapScreen::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	// Delay a tick to make sure the input is processed correctly
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this, PreDeactivateCallback](float DeltaTime)->bool
		{
			PreDeactivateCallback();

			DeactivateWidget();

			return false;
		}));
}
