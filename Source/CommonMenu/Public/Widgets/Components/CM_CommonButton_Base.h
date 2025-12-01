#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CM_CommonButton_Base.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_CommonButton_Base : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText NewButtonText) const;

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& Brush);
	
private:
	/*-- UUserWidget Interface --*/
	virtual void NativePreConstruct() override;
	/*-- End UUserWidget Interface --*/

	/*-- UCommonButtonBase Interface --*/
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	/*-- End UCommonButtonBase Interface --*/

	/*-- Bound Widgets --*/
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText{};

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	UCommonLazyImage* CommonLazyImage_ButtonImage{};
	/*-- End Bound Widgets --*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common Menu Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDisplayText{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common Menu Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDescriptionText{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common Menu Button", meta=(AllowPrivateAccess="true"))
	bool bUseUpperCaseForButtonText{false};
};
