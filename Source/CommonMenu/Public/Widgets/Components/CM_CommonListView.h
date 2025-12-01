#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "CM_CommonListView.generated.h"

class UCM_DataAsset_DataListEntryMapping;

UCLASS()
class COMMONMENU_API UCM_CommonListView : public UCommonListView
{
	GENERATED_BODY()

protected:
	/*-- UCommonListView --*/
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	/*-- End UCommonListView --*/
	
private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface
	
	UPROPERTY(EditAnywhere, Category = "Common Menu List View Settings")
	UCM_DataAsset_DataListEntryMapping* DataListEntryMapping;
};
