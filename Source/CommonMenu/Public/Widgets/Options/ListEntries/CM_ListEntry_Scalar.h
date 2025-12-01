#pragma once

#include "CoreMinimal.h"
#include "CM_ListEntry_Base.h"
#include "CM_ListEntry_Scalar.generated.h"

class UCM_ListDataObject_Scalar;
class UAnalogSlider;
class UCommonNumericTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_ListEntry_Scalar : public UCM_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	/*-- UUserWidget --*/
	virtual void NativeOnInitialized() override;
	/*-- End UUserWidget --*/

	/*-- UCM_ListEntry_Base --*/
	virtual void OnOwningListDataObjectSet(UCM_ListDataObject_Base* OwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UCM_ListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	/*-- End UCM_ListEntry_Base --*/
	
private:

	UFUNCTION()
	void OnSliderValueChanged(float NewValue);

	UFUNCTION()
	void OnSliderMouseCaptureBegin();
	
	/*-- Bound Widgets --*/
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	UCommonNumericTextBlock* CommonNumeric_SettingValue{};

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	UAnalogSlider* AnalogSlider_SettingSlider{};
	/*-- End Bound Widgets --*/

	UPROPERTY(Transient)
	UCM_ListDataObject_Scalar* CachedOwningScalarDataObject{};
};
