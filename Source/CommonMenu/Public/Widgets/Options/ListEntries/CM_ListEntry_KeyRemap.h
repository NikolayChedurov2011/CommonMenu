#pragma once

#include "CoreMinimal.h"
#include "CM_ListEntry_Base.h"
#include "CM_ListEntry_KeyRemap.generated.h"

class UCM_CommonButton_Base;
class UCM_ListDataObject_KeyRemap;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_ListEntry_KeyRemap : public UCM_ListEntry_Base
{
	GENERATED_BODY()

protected:
	/*-- UUserWidget --*/
	virtual void NativeOnInitialized() override;
	/*-- End UUserWidget --*/
	
	/*-- UCM_ListEntry_Base --*/
	virtual void OnOwningListDataObjectSet(UCM_ListDataObject_Base* OwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UCM_ListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	/*-- End IUserObjectListEntry --*/
	
private:
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();

	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyRemapCanceled(const FString& CanceledReason);
	
	/*-- Bound Widgets --*/
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	UCM_CommonButton_Base* CommonButton_RemapKey{};

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	UCM_CommonButton_Base* CommonButton_ResetKeyBinding{};
	/*-- End Bound Widgets --*/

	UPROPERTY(Transient)
	UCM_ListDataObject_KeyRemap* CachedKeyRemapDataObject{};
};
