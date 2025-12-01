#pragma once

#include "CoreMinimal.h"
#include "CM_ListDataObject_Value.h"
#include "CommonNumericTextBlock.h"
#include "CM_ListDataObject_Scalar.generated.h"

UCLASS()
class COMMONMENU_API UCM_ListDataObject_Scalar : public UCM_ListDataObject_Value
{
	GENERATED_BODY()

public:
	LIST_DATA_ACCESSOR(TRange<float>, DisplayValueRange)
	LIST_DATA_ACCESSOR(TRange<float>, OutputValueRange)
	LIST_DATA_ACCESSOR(float, SliderStepSize)
	LIST_DATA_ACCESSOR(ECommonNumericType, DisplayNumericType)
	LIST_DATA_ACCESSOR(FCommonNumberFormattingOptions, NumberFormattingOptions)

	static FCommonNumberFormattingOptions NoDecimal();
	static FCommonNumberFormattingOptions WithDecimal(int32 NumFracDigit);

	float GetCurrentValue() const;
	void SetCurrentValueFromSlider(const float NewValue);

protected:
	/*-- UCM_ListDataObject_Base --*/
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	virtual void OnEditDependencyDataModified(UCM_ListDataObject_Base* ModifiedDependencyData, const EOptionsListDataModifyReason ModifyReason);
	/*-- End UCM_ListDataObject_Base --*/
	
private:
	float StringToFloat(const FString& String) const;
	
	TRange<float> DisplayValueRange { TRange<float>(0.f, 1.f) };
	TRange<float> OutputValueRange { TRange<float>(0.f, 1.f) };
	float SliderStepSize { 0.1f };
	ECommonNumericType DisplayNumericType { ECommonNumericType::Number };
	FCommonNumberFormattingOptions NumberFormattingOptions{};
};
