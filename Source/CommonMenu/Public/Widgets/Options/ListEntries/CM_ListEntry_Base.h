#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CommonMenuTypes/CM_EnumTypes.h"
#include "CM_ListEntry_Base.generated.h"

class UCM_ListDataObject_Base;
class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(const bool bWasHovered, const bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(const bool bWasHovered);
	
protected:
	// The child blueprint widget should override this function for the gamepad
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;

	// The child blueprint widget should override it to handle the highlight state when this entry widget is hovered or selected 
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Toggle Entry Widget Highlight State"))
	void BP_OnToggleEntryWidgetHighlightState(bool bShouldHighlight) const;
	
	/*-- IUserObjectListEntry --*/
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnEntryReleased() override;
	/*-- End IUserObjectListEntry --*/

	/*-- UUserWidget --*/
	virtual FReply  NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	/*-- End UUserWidget --*/

	// The child class should override this function to handle the initialization needed. Super call is expected.
	virtual void OnOwningListDataObjectSet(UCM_ListDataObject_Base* OwningListDataObject);

	// The child class should override this function to update UI values after the data object has been modified. Super call is not needed.
	virtual void OnOwningListDataObjectModified(UCM_ListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

	virtual void OnOwningDependencyDataObjectModified(UCM_ListDataObject_Base* OwningModifiedDependencyData, EOptionsListDataModifyReason ModifyReason);
	
	// The child class should override this function to change editable state of the widgets it owns. Super call is expected.
	virtual void OnToggleEditableState(const bool bIsEditable);
	
	void SelectThisEntryWidget() const;
	
private:
	/*-- Bound Widgets --*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	UCommonTextBlock* CommonText_SettingDisplayName{};
	/*-- End Bound Widgets --*/

	UPROPERTY(Transient)
	UCM_ListDataObject_Base* CachedOwningDataObject{};
};
