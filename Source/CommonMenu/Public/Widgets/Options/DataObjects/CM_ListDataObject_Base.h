#pragma once

#include "CoreMinimal.h"
#include "CommonMenuTypes/CM_EnumTypes.h"
#include "CommonMenuTypes/CM_StructTypes.h"
#include "UObject/Object.h"
#include "CM_ListDataObject_Base.generated.h"

#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
				FORCEINLINE DataType Get##PropertyName() const {return PropertyName; } \
				void Set##PropertyName(DataType New##PropertyName) { PropertyName = New##PropertyName; }

UCLASS(Abstract)
class COMMONMENU_API UCM_ListDataObject_Base : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UCM_ListDataObject_Base*, EOptionsListDataModifyReason)
	FOnListDataModifiedDelegate OnListDataModified;
	FOnListDataModifiedDelegate OnDependencyDataModified;
	
	LIST_DATA_ACCESSOR(FName, DataID)
	LIST_DATA_ACCESSOR(FText, DataDisplayName)
	LIST_DATA_ACCESSOR(FText, DescriptionRichText)
	LIST_DATA_ACCESSOR(FText, DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)
	LIST_DATA_ACCESSOR(UCM_ListDataObject_Base*, ParentData)

	void InitDataObject();
	
	// This is empty for the base class. Child class CM_ListDataObject_Collection should override it.
	virtual TArray<UCM_ListDataObject_Base*> GetAllChildListData() const { return TArray<UCM_ListDataObject_Base*>(); }
	virtual bool HasAnyChildListData() const { return false; }

	void SetShouldApplyChangeImmediately(const bool bShouldApplyImmediately) { bShouldApplyChangeImmediately = bShouldApplyImmediately; }

	// The child class should override this to provide resetting the data
	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }

	// Gets called from OptionsDataRegistry for adding in edit conditions for the constructed list data objects
	void AddEditCondition(const FOptionsDataEditConditionDescriptor& EditCondition);

	// Gets called from OptionsDataRegistry to add dependency data
	void AddEditDependencyData(UCM_ListDataObject_Base* DependencyData);
	
	bool IsDataCurrentlyEditable();
	
protected:
	// This is empty for the base class. The child classes should override it to handle the initialization
	virtual void OnDataObjectInitialized();
	
	virtual void NotifyListDataModified(UCM_ListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);

	// The child class should override this to allow the value be set to the forced string value
	virtual bool CanSetToForcedStringValue(const FString& ForcedValue) const { return false; }

	// The child class should override this to specify how to set to the current value to the forced value
	virtual void OnSetToForcedStringValue(const FString& ForcedValue) {}

	// This function will be called when the value of the dependency data has changed. The child class can override this function to handle the custom logic needed. Super call is expected
	virtual void OnEditDependencyDataModified(UCM_ListDataObject_Base* ModifiedDependencyData, const EOptionsListDataModifyReason ModifyReason);
	
private:
	FName DataID{};
	FText DataDisplayName{};
	FText DescriptionRichText{};
	FText DisabledRichText{};
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage{};

	UPROPERTY(Transient)
	UCM_ListDataObject_Base* ParentData{};

	bool bShouldApplyChangeImmediately {false};

	UPROPERTY(Transient)
	TArray<FOptionsDataEditConditionDescriptor> EditConditionDescriptionArray{};
};
