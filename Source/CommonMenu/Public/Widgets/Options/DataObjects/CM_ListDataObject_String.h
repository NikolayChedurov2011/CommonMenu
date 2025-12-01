#pragma once

#include "CoreMinimal.h"
#include "CM_ListDataObject_Value.h"
#include "CM_ListDataObject_String.generated.h"

UCLASS()
class COMMONMENU_API UCM_ListDataObject_String : public UCM_ListDataObject_Value
{
	GENERATED_BODY()

public:
	void AddDynamicOption(const FString& StringValue, const FText& DisplayText);
	void AdvanceToNextOption();
	void BackToPreviousOption();
	void OnRotatorInitiatedValueChange(const FText& NewSelectedText);

	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	FORCEINLINE const FText& GetCurrentDisplayText() const { return CurrentDisplayText; }

protected:
	/*-- UCM_ListDataObject_Base --*/
	virtual void OnDataObjectInitialized() override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	virtual bool CanSetToForcedStringValue(const FString& ForcedValue) const  override;
	virtual void OnSetToForcedStringValue(const FString& ForcedValue) override;
	/*-- End UCM_ListDataObject_Base --*/

	bool TrySetDisplayTextFromStringValue(const FString& StringValue);
	
	FString CurrentStringValue{};
	FText CurrentDisplayText{};
	
	TArray<FString> AvailableOptionsStringArray{};
	TArray<FText> AvailableOptionsTextArray{};
	
private:
	
};

UCLASS()
class COMMONMENU_API UCM_ListDataObject_StringBool : public UCM_ListDataObject_String
{
	GENERATED_BODY()

public:
	void OverrideTrueDisplayText(const FText& NewTrueDisplayText);
	void OverrideFalseDisplayText(const FText& NewFalseDisplayText);

	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();

protected:
	/*-- UCM_ListDataObject_String --*/
	virtual void OnDataObjectInitialized() override;
	/*-- End UCM_ListDataObject_String --*/

private:
	void TryInitBoolValue();
	
	const FString TrueString { TEXT("true") };
	const FString FalseString { TEXT("false") };
};

UCLASS()
class COMMONMENU_API UCM_ListDataObject_StringEnum : public UCM_ListDataObject_String
{
	GENERATED_BODY()

public:
	template<typename EnumType>
	void AddEnumOption(EnumType EnumOption, const FText& DisplayText)
	{
		const UEnum* StaticEnumOption { StaticEnum<EnumType>() };
		const FString ConvertedEnumString { StaticEnumOption->GetNameStringByValue(EnumOption) };

		AddDynamicOption(ConvertedEnumString, DisplayText);
	}

	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption { StaticEnum<EnumType>() };
		return (EnumType)StaticEnumOption->GetValueByNameString(CurrentStringValue);
	}

	template<typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType EnumOption)
	{
		const UEnum* StaticEnumOption { StaticEnum<EnumType>() };
		const FString ConvertedEnumString { StaticEnumOption->GetNameStringByValue(EnumOption) };

		SetDefaultValueFromString(ConvertedEnumString);
	}
};

UCLASS()
class COMMONMENU_API UCM_ListDataObject_StringInteger : public UCM_ListDataObject_String
{
	GENERATED_BODY()

public:
	void AddIntegerOption(const int32 IntegerValue, const FText& DisplayText);

protected:
	/*-- UCM_ListDataObject_String --*/
	virtual void OnDataObjectInitialized() override;
	virtual void OnEditDependencyDataModified(UCM_ListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason) override;
	/*-- End UCM_ListDataObject_String --*/
};