#pragma once

#include "CoreMinimal.h"
#include "CommonMenuTypes/CM_EnumTypes.h"
#include "Widgets/CM_ActivableWidget_Base.h"
#include "CM_OptionsScreen.generated.h"

class UCM_ListDataObject_Base;
class UCM_OptionsDetailsView;
class UCM_TabListWidget_Base;
class UCM_OptionsDataRegistry;
class UCM_CommonListView;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_OptionsScreen : public UCM_ActivableWidget_Base
{
	GENERATED_BODY()

protected:
	/*-- UUserWidget --*/
	virtual void NativeOnInitialized() override;
	/*-- End UUserWidget --*/

	/*-- UCommonActivableWidget --*/
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	/*-- End UUserWidget --*/

private:
	UCM_OptionsDataRegistry* GetOrCreateDataRegistry();
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionsTabSelected(const FName TabID);

	void OnListViewItemHovered(UObject* HoveredItem, const bool bWasHovered) const;
	void OnListViewItemSelected(UObject* SelectedItem) const;

	FString TryGteEntryWidgetClassName(const UObject* OwningListItem) const;

	void OnListViewListDataModified(UCM_ListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason);
	
	/*-- Bound Widgets --*/
	UPROPERTY(meta=(BindWidget))
	UCM_TabListWidget_Base* TabListWidget_OptionsTabs{};
	
	UPROPERTY(meta=(BindWidget))
	UCM_CommonListView* CommonListView_OptionsList{};

	UPROPERTY(meta=(BindWidget))
	UCM_OptionsDetailsView* DetailsView_ListEntryInfo{};
	/*-- Bound Widgets --*/
	
	// Handle the creation of data in the options screen. Direct access to this variable is forbidden
	UPROPERTY(Transient)
	UCM_OptionsDataRegistry* CreatedOwningDataRegistry{};
	
	UPROPERTY(EditDefaultsOnly, Category = "Common Menu Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

	UPROPERTY(Transient)
	TArray<UCM_ListDataObject_Base*> ResettableDataArray{};

	bool bIsResettingData {false};  
};
