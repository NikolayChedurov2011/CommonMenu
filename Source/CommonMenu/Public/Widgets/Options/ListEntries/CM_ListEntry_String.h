#pragma once

#include "CoreMinimal.h"
#include "CM_ListEntry_Base.h"
#include "CM_ListEntry_String.generated.h"

class UCM_ListDataObject_String;
class UCM_CommonButton_Base;
class UCM_CommonRotator;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_ListEntry_String : public UCM_ListEntry_Base
{
	GENERATED_BODY()

public:

protected:
	/*-- UUserWidget --*/
	virtual void NativeOnInitialized() override;
	/*-- End UUserWidget --*/
	
	/*-- UCM_ListEntry_Base --*/
	virtual void OnOwningListDataObjectSet(UCM_ListDataObject_Base* OwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UCM_ListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	virtual void OnToggleEditableState(const bool bIsEditable) override;
	/*-- End UCM_ListEntry_Base --*/
	
private:
	void OnPreviousOptionButtonClicked()  const;
	void OnNextOptionButtonClicked()  const;

	void OnRotatorValueChanged(const int32 Value, const bool bUserInitiated) const;
	
	/*-- Bind Widgets --*/
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	UCM_CommonButton_Base* CommonButton_PreviousOption{};

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	UCM_CommonRotator* CommonRotator_AvailableOptions{};

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	UCM_CommonButton_Base* CommonButton_NextOption{};
	/*-- End Bind Widgets --*/

	UPROPERTY(Transient)
	UCM_ListDataObject_String* CachedOwningStringDataObject{};
};
