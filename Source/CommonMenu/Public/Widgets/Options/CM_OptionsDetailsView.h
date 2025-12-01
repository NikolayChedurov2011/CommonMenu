#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CM_OptionsDetailsView.generated.h"

class UCM_ListDataObject_Base;
class UCommonRichTextBlock;
class UCommonLazyImage;
class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_OptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetailsViewInfo(UCM_ListDataObject_Base* DataObject, const FString& EntryWidgetClassName = FString{});
	void ClearDetailsViewInfo() const;

protected:
	/*-- UUserWidget --*/
	virtual void NativeOnInitialized() override;
	/*-- End UUserWidget --*/
	
private:
	/*-- Bound Widgets --*/
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Title{};

	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* CommonLazyImage_DescriptionImage{};

	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_Description{};

	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DynamicDetails{};

	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DisabledReason{};
	/*-- End Bound Widgets --*/
};
